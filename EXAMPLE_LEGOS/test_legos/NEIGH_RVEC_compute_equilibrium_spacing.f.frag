!-------------------------------------------------------------------------------
!
! NEIGH_RVEC_compute_equilibrium_spacing : 
!
!    Use the Golden section search algorithm to find the equilibrium spacing by 
!    minimizing the energy of the system with respect to the periodic box size.
!
!-------------------------------------------------------------------------------
subroutine NEIGH_RVEC_compute_equilibrium_spacing(pkim, &
             DIM,CellsPerCutoff,MinSpacing,MaxSpacing,  &
             TOL,N,NNeighbors,neighborlist,RijList,     &
             verbose,RetSpacing,RetEnergy)
  use KIMservice
  implicit none
  
  !-- Transferred variables
  integer(kind=kim_intptr), intent(in)  :: pkim
  integer,                  intent(in)  :: DIM
  integer,                  intent(in)  :: CellsPerCutoff
  double precision,         intent(in)  :: MinSpacing
  double precision,         intent(in)  :: MaxSpacing
  double precision,         intent(in)  :: TOL
  integer(kind=kim_intptr), intent(in)  :: N
  integer,                  intent(in)  :: NNeighbors
  integer,                  intent(in)  :: neighborList(NNeighbors+1,N)
  double precision,         intent(in)  :: RijList(3,NNeighbors+1,N)
  logical,                  intent(in)  :: verbose
  double precision,         intent(out) :: RetSpacing
  double precision,         intent(out) :: RetEnergy
  
  !-- Local variables
  double precision,         parameter :: Golden      = (1.d0 + sqrt(5.d0))/2.d0
  integer ier
  double precision Spacings(4)
  double precision Energies(4)
  real*8 energy;           pointer(penergy,energy)
  integer N4
  real*8 coordum(DIM,1);   pointer(pcoor,coordum)
  real*8, pointer :: coords(:,:)
  real*8 cutoff;           pointer(pcutoff,cutoff)

  ! Unpack data from KIM object
  !
  penergy = kim_api_get_data_f(pkim, "energy", ier)
  if (ier.le.0) then
     call report_error(__LINE__, "kim_api_get_data_f", ier)
     stop
  endif

  pcoor = kim_api_get_data_f(pkim, "coordinates", ier)
  if (ier.le.0) then
     call report_error(__LINE__, "kim_api_get_data_f", ier)
     stop
  endif
  N4 = N  ! (Some routines expect N to be integer*4)
  call toRealArrayWithDescriptor2d(coordum, coords, DIM, N4)

  pcutoff = kim_api_get_data_f(pkim, "cutoff", ier)
  if (ier.le.0) then
     call report_error(__LINE__, "kim_api_get_data_f", ier)
     stop
  endif


  ! Initialize for minimization
  !
  Spacings(1) = MinSpacing
  ! compute new neighbor lists (could be done more intelligently, I'm sure)
  call NEIGH_RVEC_F_periodic_FCC_neighborlist(CellsPerCutoff, (cutoff+0.75), &
                                              Spacings(1), N4, NNeighbors,   &
                                              neighborList, RijList)
  call kim_api_model_compute_f(pkim, ier)
  if (ier.le.0) then
     call report_error(__LINE__, "kim_api_model_compute_f", ier)
     stop
  endif
  Energies(1) = energy
  if (verbose) &
     print *, "Energy/atom = ", Energies(1), "; Spacing = ", Spacings(1)

  ! setup and compute for max spacing
  Spacings(3) = MaxSpacing
  ! compute new neighbor lists (could be done more intelligently, I'm sure)
  call NEIGH_RVEC_F_periodic_FCC_neighborlist(CellsPerCutoff, (cutoff+0.75), &
                                              Spacings(3), N4, NNeighbors,   &
                                              neighborList, RijList)
  ! Call model compute
  call kim_api_model_compute_f(pkim, ier)
  if (ier.le.0) then
     call report_error(__LINE__, "kim_api_model_compute_f", ier)
     stop
  endif
  Energies(3) = energy
  if (verbose) &
     print *, "Energy/atom = ", Energies(3), "; Spacing = ", Spacings(3)

  ! setup and compute for first intermediate spacing
  Spacings(2) = MinSpacing + (2.0 - Golden)*(MaxSpacing - MinSpacing)
  ! compute new neighbor lists (could be done more intelligently, I'm sure)
  call NEIGH_RVEC_F_periodic_FCC_neighborlist(CellsPerCutoff, (cutoff+0.75), &
                                              Spacings(2), N4, NNeighbors,   &
                                              neighborList, RijList)
  ! Call model compute
  call kim_api_model_compute_f(pkim, ier)
  if (ier.le.0) then
     call report_error(__LINE__, "kim_api_model_compute_f", ier)
     stop
  endif
  Energies(2) = energy
  if (verbose) &
     print *, "Energy/atom = ", Energies(2), "; Spacing = ", Spacings(2)


  ! iterate until convergence.
  !
  do while (abs(Spacings(3) - Spacings(1)) .gt. TOL)
     ! set new spacing
     Spacings(4) = (Spacings(1) + Spacings(3)) - Spacings(2)
     ! compute new neighbor lists (could be done more intelligently, I'm sure)
     call NEIGH_RVEC_F_periodic_FCC_neighborlist(CellsPerCutoff, (cutoff+0.75), &
                                                 Spacings(4), N4, NNeighbors,   &
                                                 neighborList, RijList)
     ! Call model compute
     call kim_api_model_compute_f(pkim, ier)
     if (ier.le.0) then
        call report_error(__LINE__, "kim_api_model_compute_f", ier)
        stop
     endif
     Energies(4) = energy
     if (verbose) &
        print *, "Energy/atom = ", Energies(4), "; Spacing = ", Spacings(4)

     ! determine the new interval
     if (Energies(4) .lt. Energies(2)) then
        ! We want the right-hand interval
        Spacings(1) = Spacings(2); Energies(1) = Energies(2)
        Spacings(2) = Spacings(4); Energies(2) = Energies(4)
     else
        ! We want the left-hand interval
        Spacings(3) = Spacings(1); Energies(3) = Energies(1)
        Spacings(1) = Spacings(4); Energies(1) = Energies(4)
     endif
  enddo

  ! pull out results and return
  !
  RetSpacing = Spacings(2)
  RetEnergy  = Energies(2)

  return

end subroutine NEIGH_RVEC_compute_equilibrium_spacing