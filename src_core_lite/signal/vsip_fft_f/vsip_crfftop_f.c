/* Created by RJudd September 9, 1998 */
/* SPAWARSYSCEN D881 */
/**********************************************************************
// For TASP VSIPL Documentation and Code neither the United States    /
// Government, the United States Navy, nor any of their employees,    /
// makes any warranty, express or implied, including the warranties   /
// of merchantability and fitness for a particular purpose, or        /
// assumes any legal liability or responsibility for the accuracy,    /
// completeness, or usefulness of any information, apparatus,         /
// product, or process disclosed, or represents that its use would    /
// not infringe privately owned rights                                /
**********************************************************************/

#include<vsip.h>
#include<vsip_cvviewattributes_f.h>
#include<vsip_vviewattributes_f.h>
#include<vsip_fftattributes_f.h>
#include<VI_functions_f.h>
#include"VI_ft_f.h"

void vsip_crfftop_f(
           const vsip_fft_f    *Offt, 
           const vsip_cvview_f *z,
           const vsip_vview_f  *x)
{
     vsip_fft_f Nfft = *Offt;
     vsip_fft_f *fft = &Nfft;
     /* makes  where we clone some views */
     vsip_cvview_f ca1 = *z; vsip_cvview_f *Z  = &ca1;
     vsip_vview_f ra3 = *x; vsip_vview_f *x_e = &ra3; /*x*/
     vsip_vview_f ca2 = *x; vsip_vview_f *x_o = &ca2;
     vsip_cvview_f ca3 = *(fft->temp); vsip_cvview_f *y  = &ca3; /*y*/
     vsip_vview_f ra1; vsip_vview_f *y_r;
     vsip_vview_f ra2; vsip_vview_f *y_i;
     vsip_cvview_f ca4 = *(fft->temp); vsip_cvview_f *ang  = &ca4; /*ang correction*/
     /* end makes */
     /* change ang view to proper portion of temp */
     ang->offset = fft->N + 1;
     ang->length = fft->N + 1;
     /* set up views of x even and odd */
     x_e->length = fft->N;
     x_o->length = fft->N;
     x_e->stride = 2 * x->stride;
     x_o->stride = 2 * x->stride;
     x_e->offset = x->offset + x->stride;
     /* copy z to y in reverse order */
     y->stride = -1;
     y->offset = 3 * fft->N + 2;
     y->length   = fft->N + 1;
     vsip_cvcopy_f_f(z,y);
     /* reset y to normal */
     y->stride = 1;
     y->offset = 2 * fft->N + 2;
     {   /* this is where the sorting work is done */
         vsip_cvview_f *w = ang; /* these first three steps are just to make*/
         vsip_cvview_f *f = Z;   /* it easier to use already developed code */
         vsip_cvview_f *b = y;
         /* register */ vsip_length n = b->length;
         vsip_stride cwst = w->block->cstride;
         vsip_stride cfst = f->block->cstride;
         vsip_stride cbst = b->block->cstride;
         vsip_scalar_f *wpr = (vsip_scalar_f *)((w->block->R->array) + cwst * w->offset),
                       *fpr = (vsip_scalar_f *)((f->block->R->array) + cfst * f->offset),
                       *bpr = (vsip_scalar_f *)((b->block->R->array) + cbst * b->offset);
         vsip_scalar_f *wpi = (vsip_scalar_f *)((w->block->I->array) + cwst * w->offset),
                       *fpi = (vsip_scalar_f *)((f->block->I->array) + cfst * f->offset),
                       *bpi = (vsip_scalar_f *)((b->block->I->array) + cbst * b->offset);
         /* register */ vsip_stride wst = cwst * w->stride,
                                    fst = cfst * f->stride,
                                    bst = cbst * b->stride;
         vsip_scalar_f temp;
         while(n-- > 0){
             temp = /*(0.5) */ (*fpr + *bpr + *wpi * (*bpr - *fpr) - *wpr * (*fpi + *bpi));
             *bpi = /*(0.5) */ (*fpi - *bpi - *wpi * (*fpi + *bpi) + *wpr * (*fpr - *bpr));
             *bpr = temp;
             wpr += wst; wpi += wst;
             fpr += fst; fpi += fst;
             bpr += bst; bpi += bst;
         }  
      }   
      y->length--;
      VI_ccfftip_f(fft,y); /* function resides in ccfftop code */
      y_r = VI_vrealview_f(y,&ra1);
      y_i = VI_vimagview_f(y,&ra2);
      vsip_vcopy_f_f(y_r,x_o);
      vsip_vcopy_f_f(y_i,x_e);
      return;
}
