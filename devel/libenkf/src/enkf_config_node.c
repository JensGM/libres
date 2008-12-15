#include <enkf_types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stringlist.h>
#include <enkf_macros.h>
#include <enkf_config_node.h> 
#include <util.h>


struct enkf_config_node_struct {
  config_free_ftype     * freef;
  config_activate_ftype * activate;
  enkf_impl_type     	  impl_type;
  enkf_var_type      	  var_type; 

  stringlist_type       * obs_keys;       /* Keys of observations which observe this node. */
  char               	* key;
  char               	* enkf_infile;    /* Name of file which is written by forward model, and read by EnKF (not in use yet).*/
  char 		     	* enkf_outfile;   /* Name of file which is written by EnKF, and read by the forward model. */
  void               	* data;           /* This points to the config object of the actual implementation. */
};



enkf_config_node_type * enkf_config_node_alloc(enkf_var_type              var_type,
					       enkf_impl_type             impl_type,
					       const char               * key , 
					       const char               * enkf_outfile , 
					       const char               * enkf_infile  , 
					       const void               * data, 
					       config_free_ftype        * freef,
					       config_activate_ftype    * activate) {
  
  enkf_config_node_type * node = util_malloc( sizeof *node , __func__);

  node->data       	= (void *) data;
  node->freef      	= freef;
  node->activate        = activate;  
  node->var_type   	= var_type;
  node->impl_type  	= impl_type;
  node->key        	= util_alloc_string_copy(key);
  node->enkf_outfile    = util_alloc_string_copy(enkf_outfile);
  node->enkf_infile     = util_alloc_string_copy(enkf_infile);
  node->obs_keys        = stringlist_alloc_new(); 
  return node;
}


void enkf_config_node_free(enkf_config_node_type * node) {
  if (node->freef   != NULL) node->freef(node->data);
  util_safe_free(node->enkf_outfile);
  util_safe_free(node->enkf_infile);
  free(node->key);
  stringlist_free(node->obs_keys);
  free(node);
}



/**
   This is the filename used when loading from a completed forward
   model.
*/

const char * enkf_config_node_get_infile(const enkf_config_node_type * node) {
  return node->enkf_infile;
}



void *  enkf_config_node_get_ref(const enkf_config_node_type * node) { 
  return node->data; 
}



bool enkf_config_node_include_type(const enkf_config_node_type * config_node , int mask) {
  
  enkf_var_type var_type = config_node->var_type;
  if (var_type & mask)
    return true;
  else
    return false;

}


enkf_impl_type enkf_config_node_get_impl_type(const enkf_config_node_type *config_node) { 
  return config_node->impl_type; 
}


enkf_var_type enkf_config_node_get_var_type(const enkf_config_node_type *config_node) { 
  return config_node->var_type; 
}


const char * enkf_config_node_get_outfile_ref(const enkf_config_node_type * config_node) { return config_node->enkf_outfile; }
const char * enkf_config_node_get_key_ref(const enkf_config_node_type * config_node) { return config_node->key; }


const stringlist_type  * enkf_config_node_get_obs_keys(const enkf_config_node_type *config_node) {
  return config_node->obs_keys;
}


void enkf_config_node_add_obs_key(enkf_config_node_type * config_node , const char * obs_key) {
  if (!stringlist_contains(config_node->obs_keys , obs_key))
    stringlist_append_copy(config_node->obs_keys , obs_key);
}

/*****************************************************************/

VOID_FREE(enkf_config_node)
