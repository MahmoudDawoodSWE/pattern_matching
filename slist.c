#include "slist.h"

#include <stdio.h>

#include <stdlib.h>


void dbllist_init( dbllist_t *l ){//new list with size 0 head ,tail == null
    if( l == NULL ){//fail cases
        return;
    }
    dbllist_tail( l ) = NULL;
    dbllist_size( l ) = 0;
    dbllist_head( l ) = NULL;
}


void dbllist_destroy( dbllist_t *l , dbllist_destroy_t free_data ){
    if( l == NULL ){//fail cases
        return;
    }
    dbllist_node_t *temp_delete_node = dbllist_head( l );
    while ( dbllist_head( l ) != NULL ){//to run over the nodes list hold the head and move to the end
        temp_delete_node = dbllist_head( l );
        if ( free_data == DBLLIST_FREE_DATA ) {//free the data and equal to null
            free( dbllist_data( temp_delete_node ) );
            dbllist_data( temp_delete_node ) = NULL;
        }
        dbllist_head( l ) = dbllist_next( dbllist_head( l ));//move
        free( temp_delete_node );//free the node
    }
    dbllist_size( l ) = 0;//the list now empty
}


int dbllist_append( dbllist_t *l , void *data ){//add after tail
    dbllist_node_t *temp_append = ( dbllist_node_t* )malloc(sizeof( dbllist_node_t ) );//new node
    if (temp_append == NULL || l == NULL ) {//fail cases
        return -1;
    }
    if ( dbllist_size(l) != 0 ){//add to the last
        dbllist_data( temp_append ) = data;
        dbllist_next( temp_append ) = NULL;
        dbllist_next( dbllist_tail( l ) ) = temp_append;
        dbllist_prev( temp_append ) = dbllist_tail( l );
        dbllist_tail( l ) = temp_append;
    }else if ( dbllist_size( l ) == 0 ){//if the list is empty ,then head ,tail will point to temp
        dbllist_data( temp_append ) = data;
        dbllist_head( l ) = temp_append;
        dbllist_tail( l ) = temp_append;
        dbllist_next( dbllist_head( l ) ) = NULL;
        dbllist_prev( dbllist_head( l ) ) = NULL;
    }
    dbllist_size( l ) += 1;
    return 0;
}


int dbllist_prepend( dbllist_t *l , void *data){//add before head
    dbllist_node_t *temp_prepend = ( dbllist_node_t* )malloc(sizeof( dbllist_node_t ) );//new node
    if (temp_prepend == NULL || l == NULL) {//fail cases
        return -1;
    }
    if (dbllist_size(l) != 0){//add at first
        dbllist_prev( temp_prepend ) = NULL;
        dbllist_data( temp_prepend ) = data;
        dbllist_prev( dbllist_head( l ) ) = temp_prepend;
        dbllist_next( temp_prepend ) = dbllist_head(l);
        dbllist_head( l ) = temp_prepend;

    }else if ( dbllist_size( l ) == 0 ){//if the list is empty ,then head ,tail will point to temp
        dbllist_data( temp_prepend ) = data;
        dbllist_head( l ) = temp_prepend;
        dbllist_tail( l ) = temp_prepend;
        dbllist_next( dbllist_head( l ) ) = NULL;
        dbllist_prev( dbllist_head( l ) ) = NULL;
    }
    dbllist_size( l ) += 1;
    return 0;
}


int dbllist_remove( dbllist_t *l , dbllist_node_t* del_node , dbllist_destroy_t free_data  ){
    if(l == NULL || del_node == NULL ){//fail cases
        return -1;
    }
    if (free_data == DBLLIST_FREE_DATA) {//free the data of the node
        free( dbllist_data( del_node ) );
        dbllist_data( del_node ) = NULL;
    }
    if( dbllist_head( l ) == del_node  && dbllist_head( l ) == dbllist_tail( l ) && dbllist_size( l ) == 1 ){//if there is one node in the list
        dbllist_head( l ) = NULL;
        dbllist_tail( l ) = NULL;
    }else if( dbllist_tail( l ) == del_node ){//if the node is the tail
        dbllist_tail( l ) = dbllist_prev( dbllist_tail( l ) );
        dbllist_next( dbllist_tail( l ) ) = NULL;
    }else if( dbllist_head( l ) == del_node ){//if the node is the head
        dbllist_head( l ) = dbllist_next( dbllist_head( l ) );
        dbllist_prev( dbllist_head( l ) ) = NULL;
    }else{//the node somewhere in the middle
        int flag_not_found = 1;
        dbllist_node_t *node_found = dbllist_head( l );
        while (node_found){//to run over the nodes list hold the head and move to the end
            if (node_found == del_node){//check if there is node  in this list with this pointer
                flag_not_found = 0;
            }
            node_found =dbllist_next(node_found);
        }
        if(flag_not_found == 1){//the node not in the list
            return -1;
        }
        //delete the node
        dbllist_next( dbllist_prev( del_node ) ) = dbllist_next( del_node );
        dbllist_prev(dbllist_next( del_node ) ) = dbllist_prev( del_node );
    }
    free( del_node );
    dbllist_size( l ) -= 1;
    return 0;
}