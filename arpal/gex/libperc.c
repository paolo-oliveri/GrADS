#ifndef POD
#define LW
#undef lDiag
//#define lDiag

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <tgmath.h>
#include "grads.h"

#define float  double

#ifndef min
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif
#ifndef SIGN
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#endif
/*mf 971022 --- expose Mike Fiorino's global struct to these routines for warning level setting mf*/
extern struct gamfcmn mfcmn;
/*mf 971022 --- expose Mike Fiorino's global struct to these routines for warning level setting mf*/

static struct gaufb *ufba;  /* Anchor for user function defs */
char *gxgnam(char *);  /* This is also in gx.h */
static char pout[256];   /* Build error msgs here */
static char *mon[12] = {"jan","feb","mar","apr","may","jun","jul","aug","sep","oct","nov","dec"};

static int verbose = 1;   /*ams by default only print error messages and help ams*/

/* Function routine names.  Add a new function by putting the prototype here and adding to the if tests below */
gaint ffDVDbterp (struct gafunc *pfc, struct gastat *pst);
gaint ffDVDraigrid (struct gafunc *pfc, struct gastat *pst);
gaint ffDVDshift (struct gafunc *pfc, struct gastat *pst);
gaint ffDVDtimeShift (struct gafunc *pfc, struct gastat *pst);

/* prototyping */
int DVDfindOffset(gadouble, gadouble, gadouble);
void DVDfindRange(gadouble, gadouble, gadouble, gadouble, int, int, gadouble, gadouble, gadouble, gadouble, int *);
gadouble DVDintercept(gadouble, gadouble, gadouble, gadouble);
gadouble DVDfindDelta(gadouble *, int);
gadouble DVDgetValue(gadouble *, gadouble, gadouble *, gadouble *, gadouble, gadouble, int, int, gadouble, gadouble, gadouble, gadouble, gadouble);
void DVDboxAverage(gadouble *, gadouble, gadouble *, gadouble *, int, int, gadouble *, gadouble, gadouble *, gadouble *, int, int);

/* GrADS v1.x compatibility functions */
int gex_expr1(char *expr, struct gastat *pst);
int gex_setUndef (struct gastat *pst);
#define gaexpr gex_expr1
#define valprs getdbl
int comp (const void * elem1, const void * elem2);

struct gafile *getfile(char *ch, struct gastat *pst) {
	char name[20], vnam[20], cc, *pos;
	int i,fnum,ii,jj,dval,rc,dotflg,idim,jdim,dim;
	int id[4];
	float dmin[4],dmax[4],d1=0,d2;
	int gmin[4],gmax[4];
	struct gafile *pfi;
	float (*conv) (float *, float);
	float *cvals,*r,*r2,lonr,rlon,dir,spd,wrot,a,b;
	int size,j;

	/* Get the variable or function name.  It must start with a
	   letter, and consist of letters or numbers or underscore.  */
	i=0;
	while ( (*ch>='a' && *ch<='z') || (*ch>='0' && *ch<='9' ) || (*ch == '_') ) {
		name[i] = *ch;
		vnam[i] = *ch;
		ch++; i++;
		if (i>16) break;
	}
	name[i] = '\0';
	vnam[i] = '\0';  /* Save 'i' for next loop */

	/* Check for the data set number in the variable name.  If there, then this has to be a variable name */
	fnum = pst->fnum;
	dotflg=0;
	if (*ch == '.') {
		dotflg=1;
		ch++;
		pos = intprs(ch,&fnum);
		if (pos==NULL || fnum<1) {
			sprintf (pout,"Syntax error: Bad file number for variable %s \n", name);
			gaprnt (0,pout);
			return (NULL);
		}
		vnam[i] = '.';
		i++;
		while (ch<pos) {
			vnam[i] = *ch;
			ch++; i++;
		}
		vnam[i] = '\0';
	}

	/* Check for a predefined data object. */
	pfi = NULL;
	if (!dotflg) pfi = getdfn(name,pst);

	/* If not a defined grid, get a pointer to a file structure    */
	if (pfi==NULL) {
		if (!dotflg) pfi = pst->pfid;
		else {
			pfi = pst->pfi1;
			for (i=1; i<fnum && pfi!=NULL; i++) pfi = pfi->pforw;
			if (pfi==NULL) {
				gaprnt (0,"Data Request Error:  File number out of range \n");
				sprintf (pout,"  Variable = %s \n",vnam);
				gaprnt (0,pout);
				return (NULL);
			}
		}
	}
	return (pfi);
}

void gaprnt_ ( int level, char *msg ) {
	if ( level <= verbose ) gaprnt(level,msg);
}

char *nam1var[3] = {"Percentile"};
char *usg1var[3] = {"percentile\nusage: d percentile(expr,e=a,e=b,p)\n"};

/* --------------------------------------------------------
   gastat pst;
   pst->type : tipo (0=station,1=grid)
   pst->idim : varying dimension for X axis (-1=number,0=LON,1=LAT,2=LEV,3=TIM,4=EPS)
   pst->jdim : varying dimension for Y axis (-1=number,0=LON,1=LAT,2=LEV,3=TIM,4=EPS)
   pst->dimmin[5] Dimension start for each dimension (X,Y,Z,T,E) in grid units.
   pst->dimmax[5] Dimension end for each dimension (X,Y,Z,T,E) in grid units.
   pst->fnum : file number
   pst.result.pgr : pgr
   gagrid pgr;
   pst->idim : dimension for rows (-1=number,0=LON,1=LAT,2=LEV,3=TIM,4=EPS)
   pst->jdim : dimension for columns (-1=number,0=LON,1=LAT,2=LEV,3=TIM,4=EPS)
   pgr->isiz : number of elements per row (number of columns)
   pgr->jsiz : number of rows
   pgr->grid : indirizzo della griglia (isiz*jsiz array contenente i valori dell'expr)
   pgr->undef : undef value
   pgr->umask : mask for undef value in the grid
   pgr->rmin : min grid value
   pgr->rmax : max grid value
   pgr->ivals : tira fuori le coords, caricate su x1 e y1
   pgr->iavals : tira fuori le coords in grid units
   -------------------------------------------------------- */

gaint ffDVDpercentile (struct gafunc *pfc, struct gastat *pst) {
	gaint rc,isiz,jsiz,nn,error=0,i,j,z,wflag=0;
	gadouble (*conv) (gadouble *, gadouble);
	struct gagrid *pgr1,  *pgr;
	gadouble *g1,*g2,gr1,gr2;
	gadouble d2r, wt, wt1, abs;
	gadouble (*iconv) (gadouble *, gadouble);
	float percentile;
	gaint ngrid=0;
        /*data stucts*/
	gadouble **vec_perc;
	gadouble **grids;
	struct gagrid **tabpgrid;
	/*-----------*/
	gaint idx;	
	gaint  siz, dim, d, d1=0, d2, dim2, ilin, incr, bndflg;
	char *gr1u,*gr2u,*ch,*fnam,undef;
	size_t sz;
	float adding_value;
	gadouble *xgrid_out;
	struct gafile *pfi;


	/* Evaluate args number */
	if (pfc->argnum!=4) {
		gaprnt (0,"Error from Percentile: 4 arguments expected\n");
		return (1);
	}

	/* Evaluate the 1st expression, type = grid */
	rc = gaexpr(pfc->argpnt[0],pst);
	if (rc) { error=1; goto err; }
	if (pst->type!=1) {
		gaprnt (0,"Error from : The 1st argument is not a grid expression \n");
		error=1; return(1);
	}
	pfi = pst->pfid;
	ch = dimprs (pfc->argpnt[1], pst, pfi, &dim, &gr1, 1, &wflag);
	if (ch==NULL || wflag==2||dim!=4) {
		snprintf(pout,255,"Error from %s:  1st dimension expression invalid\n",fnam);
		gaprnt(0,pout);
		if (wflag==2) {
			snprintf(pout,255,"  offt expression not supported as an arg to %s\n",fnam);
			gaprnt (0,pout);
		}
		return (1);
	}
	d1 = gr1;
	ch = dimprs (pfc->argpnt[2], pst, pfi, &dim2, &gr2, 1, &wflag);
	if (ch==NULL || dim2!=dim || gr2<gr1 || wflag==2) {
		snprintf(pout,255,"Error from %s:  2nd dimension expression invalid\n",fnam);
		gaprnt(0,pout);
		if (wflag==2) {
			snprintf(pout,255,"  offt expression not supported as an arg to %s\n",fnam);
			gaprnt (0,pout);
		}
		return (1);
	}

	d2=gr2;
	ngrid=d2-d1+1;
	if (valprs(pfc->argpnt[3],&percentile)==NULL || percentile <=0 || percentile >100) { //percentile just set a this time.
          sprintf (pout,"Error from %s: Invalid Argument\n","percentile");
          gaprnt (0,pout);
          sprintf (pout,"  Expecting arg%d: %s to be a value great than 0 and less than 100 \n",4,pfc->argpnt[3]);
          gaprnt (0,pout);
          return (1);
  	}
/*Allocate memory to data structs*/	 
	grids =    (gadouble**)malloc(sizeof( gadouble* )*(ngrid));
	vec_perc = (gadouble**)malloc(sizeof( gadouble* )*(ngrid));
        tabpgrid = (struct gagrid**)malloc(sizeof(struct gagrid*)*(ngrid));
 
//   snprintf(pout,255,"  before for %d\n",d2-d1);

  /* Set pst struct to get first grid */
    conv = pfi->gr2ab[dim];
    abs = conv(pfi->grvals[dim],d1);
    pst->dmin[dim] = abs;
    pst->dmax[dim] = abs;
  if (pst->idim==dim) {          /* Fewer varying dims if user */
    pst->idim = pst->jdim;       /* averaging over varying dim */
    pst->jdim = -1;
  }
  ilin = pfi->linear[dim];
  if (pst->jdim==dim) pst->jdim = -1;
   /* Get first grid */
 for (i = 0; i < ngrid;i++){
  rc = gaexpr(pfc->argpnt[0],pst); /* Get the new status struct  */

  if (rc) return (rc);
  if (pst->type == 0) { //TODO free other memory.. use tabpgrid...
    gafree (pst);
    return(-1);
  }
   pgr1 = pst->result.pgr;
   grids[i]= pgr1->grid; /*Get the grid*/
   tabpgrid[i]=pgr1; /*save grid punt*/
   
  /* grid */
   d = d1 +i+1; 
   conv = pfi->gr2ab[dim];
   abs = conv(pfi->grvals[dim],d);
   pst->dmin[dim] = abs;
   pst->dmax[dim] = abs;
 }
    /* Get 2nd grid */
/*   rc = gaexpr(pfc->argpnt[0],pst);
  if (rc) {
    gagfre(pgr1);
    return (rc);
  }
  
 if (pst->type==0) {
    gafree(pst);
    gagfre(pgr2);
    return (-1);
  }
  pgr2 = pst->result.pgr;
*/
  
  
  isiz=pgr1->isiz;
  jsiz=pgr1->jsiz;
  undef=pgr1->undef;
  nn=isiz*jsiz;

  /* init vecs */
  xgrid_out=(float *)malloc(sizeof(float)*(nn)); //make output grid
   /* init vecs */
      for(i=0;i<isiz*jsiz;i++){
         *(xgrid_out + i)=undef;
      }

 /*evaluate the percentile idx*/
 idx=ceil(percentile/100*(ngrid))-1;
 
 snprintf(pout,255,"idx:%d\n",idx);
 gaprnt(0,pout);

 //do the job!!!
 for(i=0;i<isiz;i++){
    for(j=0;j<jsiz;j++){
       for (z=0;z<ngrid;z++){ 
        vec_perc[z]=(grids[z]+(isiz*j)+i);/*for each grid set a puntator to the value in the grid z in pont of cord i,j */   

//	snprintf(pout,255,"z:%d,i:%d,j:%d,v:%lf,",z,i,j,*(grids[z] + (isiz*j)+i));
//        gaprnt(0,pout);

//	xgrid_out[isiz*j+i]+=*(grids[z] + (isiz*j) + i);
         //(grids[z] + (isiz*j) + i);
	}      
        
       qsort(vec_perc, (sizeof(gadouble*)*ngrid)/sizeof(gadouble*),sizeof(gadouble*), comp);
       xgrid_out[isiz*j+i] = *vec_perc[idx]; /*set percentile in the grid*/


//        snprintf(pout,255,"percentile,idx %d:%lf\n",*vec_perc[idx],idx);
//        gaprnt(0,pout);

    }
 }
/*free the memory allocated by gaexpr struct for grids*/
  for(i=0;i<ngrid-1;i++){
  // snprintf(pout,225,"sooorted[]:%lf    %d\n",*vec_perc[i]);
   //        gaprnt(0,pout);
 
	gagfre(tabpgrid[i]);
  }
/*  free our memory*/

  /*set the output grid to the status struct used by grads to display our results*/
  pgr1->rmin =*vec_perc[idx];
  pgr1->grid = xgrid_out;
  
  free(vec_perc);
  free(grids);
  free(tabpgrid);
  
  pst->result.pgr=pgr1;
  
  return(0);
  
err:
  /* release memory */
  if (error) if (pst) gafree (pst); //TODO free other memory.
  if (error) 
    return 1;
  else 
    return 0;

}
/*Compare function used by qsort
!M.
*/
int comp (const void * elem1, const void * elem2)
{
    gadouble * f = *((gadouble**)elem1);
    gadouble * s = *(gadouble**)elem2;
    
    if (*f > *s) {
   //  snprintf(pout,225,"f%p,*f%lf,s%p,*s:%lf\n",f,*f,s,*s);
   //  gaprnt(0,pout);
     return 1;
    }
    if (*f < *s) {
   //  snprintf(pout,225,"f%p,*f%lf,s%p,*s:%lf\n",f,*f,s,*s);
   //  gaprnt(0,pout);

        return -1;
    }
  //   snprintf(pout,225,"f%p,*f%lf,s%p,*s:%lf\n",f,*f,s,*s);
  //   gaprnt(0,pout);
  
    return 0;
}

/* -------------------------------------------------------- */
/*
  dvd - funzione di collegamento tra grads e questa libreria
*/
int f_dvd (struct gafunc *pfc, struct gastat *pst) {

  int rc;
  char *name = pfc->argpnt[pfc->argnum];

     if ( strcmp ( "percentile" , name) == 0 ) rc = ffDVDpercentile ( pfc, pst );
     else {
       sprintf(pout,"Function '%s' not found in libperc!\n" , name);
       gaprnt(0,pout);
       return 1;
     }

    if (rc) return rc;

    /* Make sure undef mask is properly set */
    rc = gex_setUndef (pst);
    return rc; 

}

#else

=pod

=head1 NAME

libdvd.gex - Davide Sacchetti Collection of GrADS v2.0 Extensions 

=head1 SYNOPSIS

=item

display B<bterp>(I<EXPR>,I<EXPR>) -  Box averaging: same as lterp but with box averaging

=item

display B<raigrid>(I<EXPR>,I<radius>) -  expands binned data using 1/r2 weights

=item

display B<shift>(I<EXPR>,I<Xshift>,I<Yshift>) -  shift EXPR by Xshift in X and Yshift in Y

=back

=head1 AUTHORS

davide.sacchetti@arpal.gov.it, massimo.sardo@arpal.gov.it

This is free software released under the GNU General Public License;
see the source for copying conditions. There is NO warranty; not even
for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

=cut

#endif
