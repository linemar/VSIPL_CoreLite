/* $Id: vlog.t,v 1.1 2000/04/02 20:29:58 judd Exp $ */
static void vlog_f(){
   printf("********\nTEST vlog_f\n");
   {
     vsip_scalar_f data[] = {.1, .2, 1.3, 2.4, 30.0, 600, 700, 800};
     vsip_scalar_f ans[] = { -2.3026, -1.6094, 0.2624, 0.8755, 3.4012, 6.3969, 6.5511, 6.6846 };
     vsip_block_f  *block = vsip_blockbind_f(data,8,0);
     vsip_block_f  *ans_block = vsip_blockbind_f(ans,8,0);
     vsip_vview_f  *a = vsip_vbind_f(block,0,1,8);
     vsip_vview_f  *ansv = vsip_vbind_f(ans_block,0,1,8);
     vsip_vview_f  *b = vsip_vcreate_f(30,0);
     vsip_vview_f  *chk = vsip_vcreate_f(8,0);
     vsip_vputlength_f(b,8);
     vsip_blockadmit_f(block,VSIP_TRUE);
     vsip_blockadmit_f(ans_block,VSIP_TRUE);
     vsip_vlog_f(a,b);
     printf("vsip_vlog_f(a,b)\n vector a\n");VU_vprintm_f("8.6",a);
     printf("vector b\n");VU_vprintm_f("8.6",b);
     printf("expected answer to 4 decimal digits\n");VU_vprintm_f("8.4",ansv);

     vsip_vsub_f(b,ansv,chk); vsip_vmag_f(chk,chk); VU_limitIP_f(chk,.0001);
     if(vsip_vsumval_f(chk) > .5) 
         printf("error\n");
     else 
         printf("correct\n");

     printf("\na,b in place\n");
     vsip_vputstride_f(b,2);vsip_vcopy_f_f(a,b);
     vsip_vlog_f(a,a);
     vsip_vsub_f(a,ansv,chk); vsip_vmag_f(chk,chk); VU_limitIP_f(chk,.0001);
     if(vsip_vsumval_f(chk) > .5) 
         printf("error\n");
     else 
         printf("correct\n");

     vsip_valldestroy_f(a);
     vsip_valldestroy_f(b);
     vsip_valldestroy_f(chk);
     vsip_valldestroy_f(ansv);
   }
   return;
}
