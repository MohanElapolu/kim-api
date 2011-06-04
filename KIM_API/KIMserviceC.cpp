//                                                                      
// Copyright 2011 Ellad B. Tadmor, Ryan S. Elliott, and James P. Sethna 
// All rights reserved.                                                 
//                                                                     
// Author: Valeriu Smirichinski                                         
// 

#include <stdlib.h>
#include <iostream>
#include <fstream>
//#include <cctype>


#include <string.h>


using namespace std;
#include "KIMservice.h"
#include "KIMserviceC.h"

//global methods
int KIM_API_init(void * kimmdl, char *testname, char *mdlname){
    KIM_API_model * mdl;
    mdl = new KIM_API_model[1];
    if(mdl->init(testname,mdlname)) {
        *(KIM_API_model **)kimmdl = mdl;
        return 1;
    }
    *(KIM_API_model **)kimmdl=NULL;
    return 0;
}
int KIM_API_init1(void * kimmdl, char * testinputf,char * testname, char * mdlinputf,char *mdlname){
    KIM_API_model * mdl;
    mdl = new KIM_API_model[1];
    if(mdl->init(testinputf,testname,mdlinputf,mdlname)) {
        *(KIM_API_model **)kimmdl = mdl;
        return 1;
    }
    *(KIM_API_model **)kimmdl=NULL;
    return 0;
}
 
 void KIM_API_allocate(void *kimmdl, intptr_t natoms, int ntypes,int * error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    mdl->allocateinitialized(mdl,natoms,ntypes,error);
 }
void KIM_API_free(void *kimmdl,int * error){
    KIM_API_model * mdl=*(KIM_API_model **) kimmdl;
    *error=1;
    if (mdl==NULL) return;
    mdl->free(error);
    delete [] mdl;
    *(KIM_API_model **) kimmdl=NULL;

}
void KIM_API_print(void *kimmdl,int * error){
    *error =0;
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    if (mdl==NULL) return;
    cout<<(*mdl);
    *error=1;
}
int KIM_API_model_init(void * kimmdl){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    if(mdl->model_init()) return 1;
    return 0;
}
void KIM_API_model_compute(void *kimmdl,int *error){
 
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;

    mdl->model_compute(error);

}
void KIM_API_model_destroy(void * kimmdl,int *error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    mdl->model_destroy(error);
}
void KIM_API_get_Units(void *kimmdl,char * UnitsSystem,int *error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    mdl->get_units(UnitsSystem,error);
}
void KIM_API_get_originalUnits(void *kimmdl,char * UnitsSystem,int *error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    mdl->get_originalUnits(UnitsSystem,error);
}
int KIM_API_set_Units(void *kimmdl,char * UnitsSystem){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
cout<<"UnitsSystem"<<":"<<endl;
    if(mdl->set_units(UnitsSystem)) return 1;
    return 0;
}
void KIM_API_transform_Units_to(void *kimmdl,char * UnitsSystem,int *error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    mdl->transform_units_to(UnitsSystem,error);
}
int KIM_API_isUnitS_fixed(void *kimmdl){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    if(mdl->is_unitsfixed()) return 1;
    return 0;
}
void * KIM_API_get_listAtomsTypes(void * kimmdl,int* nATypes, int* error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    return mdl->get_listAtomsTypes(nATypes,error);
}

void * KIM_API_get_listParams(void * kimmdl,int* nVpar, int* error){
     KIM_API_model * mdl=(KIM_API_model *) kimmdl;
     return mdl->get_listParams(nVpar,error);
}
void * KIM_API_get_listFreeParams(void * kimmdl,int* nVpar, int* error){
     KIM_API_model * mdl=(KIM_API_model *) kimmdl;
     return mdl->get_listFreeParams(nVpar,error);
}
void * KIM_API_get_listFixedParams(void * kimmdl,int* nVpar, int* error){
     KIM_API_model * mdl=(KIM_API_model *) kimmdl;
     return mdl->get_listFixedParams(nVpar,error);
}
void * KIM_API_get_NBC_method(void *kimmdl,int * error){
     KIM_API_model * mdl=(KIM_API_model *) kimmdl;
     return mdl->get_NBC_method(error);
}
int KIM_API_get_aTypeCode(void * kimmdl, char* atom, int * error){
     KIM_API_model * mdl=(KIM_API_model *) kimmdl;
     return mdl->get_aTypeCode(atom,error);
}

int KIM_API_get_full_neigh(void *kimmdl,int mode,int request,
        int *atom, int *numnei, int **nei1atom, double **Rij){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    return mdl->get_full_neigh(mode,request,atom,numnei,nei1atom,Rij);
}
int KIM_API_get_half_neigh(void *kimmdl,int mode,int request,
        int *atom, int *numnei, int **nei1atom, double **Rij){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    return mdl->get_half_neigh(mode,request,atom,numnei,nei1atom,Rij);
}


//element access methods by name
int  KIM_API_set_data(void *kimmdl,char *nm, intptr_t size, void *dt){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    if(mdl->set_data(nm,size,dt)) return 1;
    return 0;
}
void * KIM_API_get_data(void *kimmdl,char *nm,int *error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    return mdl->get_data(nm,error);
}

intptr_t KIM_API_get_size(void *kimmdl,char *nm,int *error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    return mdl->get_size(nm,error);
  
}
intptr_t KIM_API_get_rank_shape(void *kimmdl,char *nm, int * shape, int *error){
    
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    return mdl->get_rank_shape(nm,shape,error);
    
}
void KIM_API_set2_compute(void *kimmdl,char *nm, int * error){
    *error = 0;
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    mdl->set2_compute(nm);
    *error = 1;
}
void KIM_API_set2_uncompute(void *kimmdl,char *nm, int * error){
    *error = 0;
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    mdl->set2_uncompute(nm);
    *error = 1;
}
int KIM_API_isit_compute(void *kimmdl,char *nm, int * error){
    *error =0;
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    return (*mdl)[nm].flag->calculate;
    *error =1;
}

// element access by Index (fast way)
int KIM_API_get_index(void *kimmdl,char *nm, int *error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    return mdl->get_index(nm,error);
}
void  KIM_API_set_data_byI(void *kimmdl,int I, intptr_t size, void *dt, int * error){
     KIM_API_model * mdl=(KIM_API_model *) kimmdl;
     *error =0;
     if (mdl == NULL) return;

      int c=1;
       (*mdl)[I].data = dt;
       (*mdl)[I].size = size;
        if ((*mdl)[I].rank > 1) {
            for (int i=1;i<(*mdl)[I].rank;i++) {
                c=c * (*mdl)[I].shape[i];
            }
            (*mdl)[I].shape[0] = size/c;
        }
        (*mdl)[I].flag->freeable = 1;
        *error=1;
}
void * KIM_API_get_data_byI(void *kimmdl,int I,int *error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    *error =0;
    if (mdl == NULL) return NULL;
    *error =1;
    return (*mdl)[I].data;
}

intptr_t KIM_API_get_size_byI(void *kimmdl,int I, int *error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    *error =0;
    if (mdl == NULL) return 0;
    *error =1;
    return (*mdl)[I].size;
    
}
intptr_t KIM_API_get_rank_shape_byI(void *kimmdl,int I, int * shape,int *error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    *error =0;
    if (mdl == NULL) return -2;
    *error =1;
     if((*mdl)[I].rank == 0){
            return 0;
        }else if((*mdl)[I].rank ==1){
            shape[0] = (int)(*mdl)[I].size;
            return 1;
        }else if((*mdl)[I].rank>1){
            for (int i=0; i< (*mdl)[I].rank; i++) shape[i] =(*mdl)[I].shape[i];
            return (*mdl)[I].rank;
        }else{
            *error =0;
            return -1;
        }
}

void KIM_API_set2_compute_byI(void *kimmdl,int I,int * error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    *error =0;
    if (mdl == NULL) return ;
    (*mdl)[I].flag->calculate =1;
     *error =1;
}
void KIM_API_set2_uncompute_byI(void *kimmdl,int I,int * error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    *error =0;
    if (mdl == NULL) return ;
    (*mdl)[I].flag->calculate = 0;
    *error =1;
}
int KIM_API_isit_compute_byI(void *kimmdl,int I,int *error){
    KIM_API_model * mdl=(KIM_API_model *) kimmdl;
    *error =0;
    if (mdl == NULL) return 1;
    *error =1;
    return (*mdl)[I].flag->calculate;
    
}

float KIM_API_get_unit_scalefactor(void * kim, char*nm,int *error){
    KIM_API_model * mdl=(KIM_API_model *) kim;
    return mdl->get_unit_scalefactor(nm,error);
}

//Fortran interface
//global methots
int kim_api_init_(void * kimmdl,char ** testname, char **mdlname){
    return KIM_API_init(kimmdl,*testname,*mdlname);
}
int kim_api_init1_(void * kimmdl, char ** testinputf,char ** testname, char ** mdlinputf,char **mdlname){
    return KIM_API_init1(kimmdl,*testinputf,*testname,*mdlinputf,*mdlname);
}
void kim_api_allocate_(void *kimmdl, intptr_t *natoms, int *ntypes,int * error){
    KIM_API_allocate(*(KIM_API_model **)kimmdl,*natoms,*ntypes,error);
}
void kim_api_free_(void *kimmdl,int *error){
    KIM_API_free(kimmdl,error);
}
void kim_api_print_(void *kimmdl,int *error){
    KIM_API_print(*(KIM_API_model **)kimmdl, error);
}
void kim_api_model_compute_(void*kimmdl,int *error){
    KIM_API_model_compute(*(KIM_API_model **)kimmdl,error);
}

void kim_api_model_destroy_(void * kimmdl,int *error){
    KIM_API_model_destroy(*(KIM_API_model **)kimmdl,error);
}
int kim_api_model_init_(void * kimmdl){
    return KIM_API_model_init(*(KIM_API_model **)kimmdl);
}


void kim_api_get_units_(void *kimmdl,char ** UnitsSystem, int *error){
    KIM_API_get_Units(*(KIM_API_model **)kimmdl,*UnitsSystem,error);
    for(int i=strlen(*UnitsSystem)+1;i<KEY_CHAR_LENGTH;i++){
        (*UnitsSystem)[i]='\0';
    }
}
void kim_api_get_originalunits_(void *kimmdl,char ** UnitsSystem,int *error){
    KIM_API_get_originalUnits(*(KIM_API_model **)kimmdl,*UnitsSystem, error);
    for(int i=strlen(*UnitsSystem)+1;i<KEY_CHAR_LENGTH;i++){
        (*UnitsSystem)[i]='\0';
    }
}
int kim_api_set_units_(void *kimmdl,char ** UnitsSystem){

    return KIM_API_set_Units(*(KIM_API_model **)kimmdl,*UnitsSystem);
}
void kim_api_transform_units_to_(void *kimmdl,char ** UnitsSystem,int * error){

    KIM_API_transform_Units_to(*(KIM_API_model **)kimmdl,*UnitsSystem,error);

}
int kim_api_isunits_fixed_(void *kimmdl){
    return KIM_API_isUnitS_fixed(*(KIM_API_model **)kimmdl);
}
void * kim_api_get_listatomstypes_(void * kimmdl,int* nATypes, int* error){
 return KIM_API_get_listAtomsTypes(*(KIM_API_model **)kimmdl,nATypes,error);
}
void * kim_api_get_listparams_(void * kimmdl,int* nVpar, int* error){
 return KIM_API_get_listParams(*(KIM_API_model **)kimmdl,nVpar,error);
}
void * kim_api_get_listfreeparams_(void * kimmdl,int* nVpar, int* error){
 return KIM_API_get_listFreeParams(*(KIM_API_model **)kimmdl,nVpar,error);
}
void * kim_api_get_listfixedparams_(void * kimmdl,int* nVpar, int* error){
 return KIM_API_get_listFixedParams(*(KIM_API_model **)kimmdl,nVpar,error);
}
void * kim_api_get_nbc_method_(void * kimmdl,int* error){
    return KIM_API_get_NBC_method(*(KIM_API_model **)kimmdl,error);
}
int kim_api_get_atypecode_(void * kimmdl, char **atom, int * error){
 return KIM_API_get_aTypeCode(*(KIM_API_model **)kimmdl,*atom,error);
}

int kim_api_get_full_neigh_(void *kimmdl,int *mode,int *request,
        int *atom, int *numnei, int **nei1atom, double **Rij){
    return KIM_API_get_full_neigh(*(KIM_API_model **)kimmdl, *mode, *request,
            atom,numnei,nei1atom,Rij);
}

int kim_api_get_half_neigh_(void *kimmdl,int *mode,int *request,
        int *atom, int *numnei, int **nei1atom, double **Rij){
    return KIM_API_get_half_neigh(*(KIM_API_model **)kimmdl, *mode, *request,
            atom,numnei,nei1atom,Rij);
}

//element access methods
int  kim_api_set_data_(void *kimmdl,char **nm,  intptr_t *size, void *dt){
    return KIM_API_set_data(*(KIM_API_model **)kimmdl,*nm,*size,*(char**)dt);
}

void * kim_api_get_data_(void *kimmdl,char **nm,int *error){
    return KIM_API_get_data(*(KIM_API_model **)kimmdl,*nm,error);
}
void * kim_api_get_data_cptr_(void *kimmdl,char **nm, int *error){
    return KIM_API_get_data(*(KIM_API_model **)kimmdl,*nm,error);
}

intptr_t kim_api_get_size_(void *kimmdl,char **nm,int *error){
    return KIM_API_get_size(*(KIM_API_model **)kimmdl,*nm,error);
}
intptr_t kim_api_get_rank_shape_(void *kimmdl,char **nm, int ** shape,int *error){
    return KIM_API_get_rank_shape(*(KIM_API_model **)kimmdl,*nm,*shape,error);
}

void kim_api_set2_compute_(void *kimmdl,char **nm,int *error){
    KIM_API_set2_compute(*(KIM_API_model **)kimmdl,*nm,error);
}
void kim_api_set2_uncompute_(void *kimmdl,char **nm,int *error){
    KIM_API_set2_uncompute(*(KIM_API_model **)kimmdl,*nm,error);
}
int kim_api_isit_compute_(void *kimmdl,char **nm,int *error){
    return KIM_API_isit_compute(*(KIM_API_model **)kimmdl,*nm,error);
}

int kim_api_get_index_(void *kimmdl,char **nm,int *error){
    return KIM_API_get_index(*(KIM_API_model **)kimmdl,*nm,error);
}

void kim_api_set_data_byi_(void *kimmdl,int * I, intptr_t * size, void *dt,int *error){
    KIM_API_set_data_byI(*(KIM_API_model **)kimmdl,*I,*size,*(char**)dt,error);
}
void * kim_api_get_data_byi_(void *kimmdl,int * I,int *error){
    return KIM_API_get_data_byI(*(KIM_API_model **)kimmdl,*I,error);
}

intptr_t kim_api_get_size_byi_(void *kimmdl,int * I,int *error){
    return KIM_API_get_size_byI(*(KIM_API_model **)kimmdl,*I,error);
}
intptr_t kim_api_get_rank_shape_byi_(void *kimmdl,int * I, int ** shape,int *error){
    return KIM_API_get_rank_shape_byI(*(KIM_API_model **)kimmdl,*I,*shape,error);
}

void kim_api_set2_compute_byi_(void *kimmdl,int * I,int *error){
    KIM_API_set2_compute_byI(*(KIM_API_model **)kimmdl,*I,error);
}
void kim_api_set2_uncompute_byi_(void *kimmdl,int * I, int *error){
    KIM_API_set2_uncompute_byI(*(KIM_API_model **)kimmdl,*I,error);
}
int kim_api_isit_compute_byi_(void *kimmdl,int * I,int *error){
    return KIM_API_isit_compute_byI(*(KIM_API_model **)kimmdl,*I,error);
}

float kim_api_get_unit_scalefactor_(void * kim, char**nm, int *error){
    return KIM_API_get_unit_scalefactor(*(KIM_API_model **)kim, *nm,error);
}