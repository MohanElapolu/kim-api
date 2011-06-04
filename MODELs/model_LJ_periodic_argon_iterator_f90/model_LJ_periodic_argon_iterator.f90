!****************************************************************************
!**
!**  MODULE model_LJ_periodic_argon_iterator
!**
!**  Lennard-Jones pair potential model for argon -- iterator version
!**  (modified to have smooth cutoff)
!**
!**  Author: Ellad B. Tadmor
!**  Date  : 17 Mar 2011
!**
!**  Copyright 2011 Ellad B. Tadmor and Ronald E. Miller
!**  All rights reserved.
!**
!****************************************************************************

module model_LJ_periodic_argon_iterator

use KIMservice
implicit none

save
private
public Compute_Energy_Forces, &
       model_cutoff

! Model parameters
double precision, parameter :: model_cutoff  = 8.15d0   ! cutoff distance
double precision, parameter :: model_epsilon = 0.0104d0 ! LJ epsilon
double precision, parameter :: model_sigma   = 3.4d0    ! LJ sigma

! Compute parameters A,B,C for smoothing function: A r^2 + B r + C
double precision, parameter :: model_cutnorm = model_cutoff/model_sigma
double precision, parameter :: &
   model_A = 12.d0*model_epsilon*(-26.d0 + 7.d0*model_cutnorm**6)/ &
            (model_cutnorm**14*model_sigma**2)
double precision, parameter :: &
   model_B = 96.d0*model_epsilon*(7.d0-2.d0*model_cutnorm**6)/     &
            (model_cutnorm**13*model_sigma)
double precision, parameter :: &
   model_C = 28.d0*model_epsilon*(-13.d0+4.d0*model_cutnorm**6)/   &
            (model_cutnorm**12)

! Optimization paramaters
double precision, parameter :: model_sigmasq = model_sigma**2
double precision, parameter :: model_cutsq   = model_cutoff**2

contains

!-------------------------------------------------------------------------------
!
! Compute energy and forces on atoms from the positions.
!
!-------------------------------------------------------------------------------
subroutine Compute_Energy_Forces(pkim,ier)
implicit none

!-- Transferred variables
integer(kind=kim_intptr), intent(in) :: pkim
integer,                  intent(out) :: ier

!-- Local variables
integer, parameter :: DIM=3
double precision, dimension(DIM) :: Sij,Rij
double precision :: r,Rsqij,phi,dphi,d2phi
integer :: i,j,jj,numnei,atom,request

!-- KIM variables
integer(kind=8) N; pointer(pN,N)
real*8 energy; pointer(penergy,energy)
real*8 coordum(DIM,1); pointer(pcoor,coordum)
real*8 forcedum(DIM,1); pointer(pforce,forcedum)
real*8 enepotdum(1); pointer(penepot,enepotdum)
real*8 boxlength(3); pointer(pboxlength,boxlength)
real*8, pointer :: coor(:,:),force(:,:),ene_pot(:)
real*8 virial; pointer(pvirial,virial)
real*8 Rij_dummy(3,1); pointer(pRij_dummy,Rij_dummy)
integer nei1atom(1); pointer(pnei1atom,nei1atom)
integer N4     !@@@@@@@@@ NEEDS TO BE FIXED

! Unpack data from KIM object
!
pN         = kim_api_get_data_f(pkim,"numberOfAtoms",ier); if (ier.le.0) return
penergy    = kim_api_get_data_f(pkim,"energy",ier);        if (ier.le.0) return
pcoor      = kim_api_get_data_f(pkim,"coordinates",ier);   if (ier.le.0) return
pforce     = kim_api_get_data_f(pkim,"forces",ier);        if (ier.le.0) return
penepot    = kim_api_get_data_f(pkim,"energyPerAtom",ier); if (ier.le.0) return
pvirial    = kim_api_get_data_f(pkim,"virial",ier);        if (ier.le.0) return
pboxlength = kim_api_get_data_f(pkim,"boxlength",ier);     if (ier.le.0) return
N4=N
call toRealArrayWithDescriptor2d(coordum,coor,DIM,N4)
call toRealArrayWithDescriptor2d(forcedum,force,DIM,N4)
call toRealArrayWithDescriptor1d(enepotdum,ene_pot,N4)

!  Initialize potential energies, forces, virial term
!
ene_pot(1:N) = 0.d0
force(1:3,1:N) = 0.d0
virial = 0.d0

! Reset neighbor iterator to beginning
!
request = 0
ier = kim_api_get_half_neigh(pkim,0,request,atom,numnei,pnei1atom,pRij_dummy)
if (ier.ne.2) return  ! reset failed

!  Use iterator to loop over all atoms and their neighbors
!
do
   ! increment iterator
   request = 1 ! request next atom
   ier = kim_api_get_half_neigh(pkim,0,request,atom,numnei,pnei1atom,pRij_dummy)
   if (ier.eq.0) exit ! no more atoms in the list
   if (ier.lt.0) return
   i = atom
   do jj= 1, numnei
      j = nei1atom(jj)
      Rij = coor(:,i) - coor(:,j)                 ! distance vector between i j
      where ( abs(Rij) > 0.5d0*boxlength )        ! periodic boundary conditions
         Rij = Rij - sign(boxlength,Rij)          ! applied where needed.
      end where                                   ! 
      Rsqij = dot_product(Rij,Rij)                ! compute square distance
      if ( Rsqij < model_cutsq ) then             ! particles are interacting?
         r = sqrt(Rsqij)                          ! compute distance
         call pair(r,phi,dphi,d2phi)              ! compute pair potential
         ene_pot(i) = ene_pot(i) + 0.5d0*phi      ! accumulate energy
         ene_pot(j) = ene_pot(j) + 0.5d0*phi      ! (i and j share it)
         virial = virial + r*dphi                 ! accumul. virial=sum r(dV/dr)
         force(:,i) = force(:,i) - dphi*Rij/r     ! accumulate forces
         force(:,j) = force(:,j) + dphi*Rij/r     !    (Fji = -Fij)
      endif
   enddo

enddo
virial = - virial/DIM                             ! definition of virial term
energy = sum(ene_pot(1:N))                        ! compute total energy
ier = 1

end subroutine Compute_Energy_Forces

!-------------------------------------------------------------------------------
!
! Pair potential: Lennard-Jones with smooth cutoff imposed by Ar^2 + Br + C
!
!-------------------------------------------------------------------------------
subroutine pair(r,phi,dphi,d2phi)
implicit none

!-- Transferred variables
double precision, intent(in)  :: r
double precision, intent(out) :: phi, dphi, d2phi

!-- Local variables
double precision :: rsq,sor,sor6,sor12

rsq  = r*r             !  r^2
sor  = model_sigma/r   !  (sig/r)
sor6 = sor*sor*sor
sor6 = sor6*sor6       !  (sig/r)^6
sor12= sor6*sor6       !  (sig/r)^12

phi   =  4.d0*model_epsilon*(sor12-sor6) + model_A*rsq + model_B*r + model_C
dphi  = 24.d0*model_epsilon*(-2.d0*sor12+sor6)/r  + 2.d0*model_A*r + model_B
d2phi = 24.d0*model_epsilon*(26.d0*sor12-7.d0*sor6)/rsq + 2.d0*model_A

end subroutine pair

end module model_LJ_periodic_argon_iterator

!-------------------------------------------------------------------------------
!
! Model initialization routine (REQUIRED)
!
!-------------------------------------------------------------------------------
subroutine model_LJ_periodic_argon_iterator_f90_init(pkim)
use model_LJ_periodic_argon_iterator
use KIMservice
implicit none

!-- Transferred variables
integer(kind=kim_intptr), intent(in) :: pkim

!-- Local variables
integer(kind=kim_intptr), parameter :: sz=1
real*8 cutoff; pointer(pcutoff,cutoff)
integer ier

! store pointer to compute function in KIM object
if (kim_api_set_data_f(pkim,"compute",sz,loc(Compute_Energy_Forces)).ne.1) &
   stop '* ERROR: compute keyword not found in KIM object.'

! store model cutoff in KIM object
pcutoff =  kim_api_get_data_f(pkim,"cutoff",ier)
if (ier.le.0) stop '* ERROR: cutoff not found in KIM object.'
cutoff = model_cutoff

end subroutine model_LJ_periodic_argon_iterator_f90_init
