/**\file */
#ifndef SLIC_DECLARATIONS_ChromaKey_H
#define SLIC_DECLARATIONS_ChromaKey_H
#include "MaxSLiCInterface.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define Simple_PCIE_ALIGNMENT (16)



void ChromaKey(
	int32_t param_N,
	int32_t key_cb_a,
	int32_t key_cr_b,
	double tol_a_c,
	double tol_b_d,
	uint8_t *in_image_x,
	uint8_t *bg_image_y,
	uint8_t *out_image_z);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* SLIC_DECLARATIONS_Simple_H */

