#include "pattern_matching.h"

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

void recursive_free( pm_state_t* node );
int setup_state( pm_state_t* , pm_int_t , pm_int_t );




int setup_state( pm_state_t* set_val_node , pm_int_t the_depth_node , pm_int_t the_id_node ){
    if( set_val_node == NULL )
        return -1;
    //start the _transitions list
    set_val_node -> _transitions = ( dbllist_t* )malloc( sizeof( dbllist_t ) );
    if ( set_val_node -> _transitions == NULL )
        return -1;
    dbllist_init( set_val_node -> _transitions );
    //start the output list
    set_val_node -> output = ( dbllist_t* )malloc( sizeof( dbllist_t ) );
    if(set_val_node -> output == NULL) {
        return -1;
    }
    dbllist_init( set_val_node -> output );
    //set the val of the new node
    set_val_node -> id = the_id_node;
    set_val_node -> fail = NULL;
    set_val_node -> depth = the_depth_node;
    return 0;
}




int pm_init( pm_t *tree ){
    if(tree == NULL)
        return -1;
    tree -> zerostate = ( pm_state_t* ) malloc( sizeof( pm_state_t ) );
    if (tree -> zerostate == NULL)
        return -1;
    if ( setup_state(tree -> zerostate,0,0) == -1 ){
        free(tree -> zerostate);
        if(tree -> zerostate ->_transitions != NULL)
            free(tree -> zerostate ->_transitions);
        if(tree -> zerostate ->output != NULL)
            free(tree -> zerostate -> output);
        return -1;
    }
    tree -> newstate = 0;
    return 0;
}




int pm_addstring(pm_t *tree,unsigned char *text, _size_t length_text){
    int j = 0;
    pm_state_t* state = tree -> zerostate;
    while( pm_goto_get( state, text[j] ) != NULL ){//there is a way "states"
        state = pm_goto_get( state, text[j] );
        j = j + 1;
    }
    pm_state_t *new_state_to_add;
    for( int p = j; p < length_text; p++ ){//there is no way "states" create it
        tree -> newstate +=1;
        //new state
        new_state_to_add = ( pm_state_t* )malloc( sizeof( pm_state_t ) );
        if ( new_state_to_add == NULL )
        {
            pm_destroy( tree );
            return -1;
        }
        //start the new state
        if (setup_state( new_state_to_add , state -> depth + 1 , tree -> newstate ) == -1)
        {
            pm_destroy( tree );
            return -1;
        }
        //make the link
        if (pm_goto_set( state , text[p] , new_state_to_add ) == -1)
        {
            pm_destroy( tree );
            return -1;
        }
        // print the data
        printf( "Allocating state %d\n", tree -> newstate );
        printf( "%d -> %c -> %d\n", state -> id, text[p], tree -> newstate );
        state = new_state_to_add;
    }
    if( dbllist_append(state -> output,text) == -1 ){//add  the output option "text"
        pm_destroy( tree );
        return -1;
    }
    return 0;
}




int pm_goto_set( pm_state_t *state_1 , unsigned char arrow_label , pm_state_t *state_2 ){//link state 1 to state 2
    pm_labeled_edge_t *arrow = ( pm_labeled_edge_t* )malloc( sizeof( pm_labeled_edge_t ) );
    if ( arrow == NULL )
    {
        return -1;
    }
    arrow -> label = arrow_label;
    arrow -> state = state_2;
    if ( dbllist_prepend(state_1 -> _transitions , arrow ) == -1)
    {
        free( arrow );
        return -1;
    }
    return 0;
}




pm_state_t* pm_goto_get( pm_state_t *state , unsigned char arrow_label ){//find black arrow with  "arrow_label" label
    if( state ==NULL )
    {
        return NULL;
    }
    dbllist_node_t *head_arrows = dbllist_head( state->_transitions );
    pm_labeled_edge_t *find_arrow;
    while (head_arrows != NULL)
    {
        find_arrow =( pm_labeled_edge_t* )dbllist_data( head_arrows );
        if ( find_arrow -> label == arrow_label )//if we found arrow with this label
            return find_arrow -> state;
        head_arrows = dbllist_next( head_arrows );
    }
    return NULL;//if we arrive here this mean there is no arrow with this label
}




int pm_makeFSM( pm_t *tree ){
    dbllist_t* queue = ( dbllist_t* )malloc(sizeof( dbllist_t ));
    if(queue == NULL){
        pm_destroy( tree );
        return -1;
    }
    dbllist_init( queue );
    // to run over the first level node
    dbllist_node_t *arrows = dbllist_head( tree -> zerostate -> _transitions );
    pm_state_t *s;
    pm_state_t *r;
    pm_state_t *fail;
    unsigned char label;
    while( arrows != NULL ){
        s = ( ( pm_labeled_edge_t* )dbllist_data( arrows ) ) -> state;//take state from level 1

        if(dbllist_append(queue, s) == -1 )//insert level 1 state into the queue
            return -1;
        s->fail = tree ->zerostate;//failure  of stats on level 1 are the state 0
        arrows = dbllist_next( arrows );
    }
    while( dbllist_size( queue ) != 0 ){
        r = ( pm_state_t* )dbllist_data( dbllist_head( queue ) );
        if(dbllist_remove( queue , dbllist_head( queue ) , DBLLIST_LEAVE_DATA) == -1 ) {
             pm_destroy( tree );
            return -1;
        }
        arrows = dbllist_head(r -> _transitions);
        while(arrows != NULL){
            s = ( ( pm_labeled_edge_t* )dbllist_data( arrows ) ) -> state;
            if (dbllist_append(queue , s) ==  -1){//add to the end of the queue
                pm_destroy( tree );
                return -1;
            }
            fail = r -> fail;
            label = ( ( pm_labeled_edge_t* )dbllist_data( arrows ) ) -> label;
            pm_state_t *find_new_way = pm_goto_get(fail , label);
            while (find_new_way == NULL)
            {
                if (fail == NULL){
                    find_new_way = tree -> zerostate;//no way
                    break;
                }
                fail = fail -> fail;//try new fail
                find_new_way = pm_goto_get( fail , label );//check if there is way
            }
            printf( "Setting f(%d) = %d\n" , s -> id , find_new_way -> id );
            s -> fail = find_new_way;//find new way
            //add list to list
            dbllist_node_t *tacker = dbllist_head( s -> fail -> output );
            while( tacker != NULL )
            {
                if( dbllist_append( s -> output, dbllist_data( tacker )) == -1 ){
                    pm_destroy( tree );
                    return -1;
                }
                tacker = dbllist_next( tacker );
            }
            arrows = dbllist_next( arrows );
        }
    }
    dbllist_destroy( queue , DBLLIST_FREE_DATA );
    free(queue);
    return 0;
}




dbllist_t* pm_fsm_search( pm_state_t * root , unsigned char * str , _size_t length_str ){
    if(root == NULL || str == NULL)
        return NULL;
    dbllist_t *matches = ( dbllist_t* )malloc( sizeof( dbllist_t ) );
    if(matches == NULL)
        return NULL;
    dbllist_init( matches );
    pm_state_t *find_new_way;
    pm_state_t *state = root;
    dbllist_node_t *list_node;
    pm_match_t *match;
    for( int j = 0; j < length_str; j++ ){//find way
        find_new_way = pm_goto_get(state, str[j]);
        while ( find_new_way == NULL){
            state = state -> fail;
            if (state == NULL){
                find_new_way = root;
                break;
            }
            find_new_way = pm_goto_get( state, str[j] );
        }
        state = find_new_way;//set new  way
        //on the way we take the matches
        list_node = dbllist_head( state -> output );
        while( list_node != NULL ){
            match = ( pm_match_t* )malloc( sizeof(pm_match_t) );
            if( match == NULL )
                return NULL;
            match -> pattern = ( char* ) list_node -> data;
            match -> end_pos = j;
            match -> start_pos = match -> end_pos - ( int )strlen(( char* )match -> pattern ) + 1;
            match -> fstate = state;
            if( dbllist_append(matches, match) == -1 )
                return NULL;
            list_node = dbllist_next( list_node );
        }
    }
    return matches;
}



// recursive the method call itself to the sons and free sons data
void pm_destroy( pm_t* tree ) {
    recursive_free(tree -> zerostate );//call the recursive_free with the root
    //destroy the root data
    dbllist_destroy( tree -> zerostate -> _transitions  , DBLLIST_FREE_DATA);
    dbllist_destroy(tree -> zerostate -> output , DBLLIST_LEAVE_DATA);
    free(tree -> zerostate -> _transitions);
    free(tree -> zerostate -> output);
    free(tree -> zerostate);

}



void recursive_free( pm_state_t* node ){
    dbllist_node_t* grandsons_states = dbllist_head( node -> _transitions );
    pm_state_t* grandson;
    while( grandsons_states != NULL ){//call the function to the sons
        grandson =  ( ( pm_labeled_edge_t* )dbllist_data( grandsons_states ) ) -> state;
        recursive_free(grandson );
        grandsons_states = dbllist_next( grandsons_states );
        dbllist_destroy( grandson -> _transitions , DBLLIST_FREE_DATA );
        dbllist_destroy( grandson -> output , DBLLIST_LEAVE_DATA );
        free(grandson -> _transitions);
        free(grandson -> output);
        free( grandson );
    }
}
