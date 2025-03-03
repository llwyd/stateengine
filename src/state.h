/*
 *
 * FSM Engine
 *
 * T.L. 2022
 *
 */

#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>
#include <stdbool.h>

/* Core State Machine Defines and helper macros */
#define DEFAULT_EVENTS(EVNT) \
    EVNT( None ) \
    EVNT( Enter ) \
    EVNT( Exit ) \

#define STATE_RETURN_CODES \
    RETURN_CODE( None ) \
    RETURN_CODE( Handled ) \
    RETURN_CODE( Unhandled ) \
    RETURN_CODE( Transition ) \

#ifndef MAX_NESTED_STATES
#define MAX_NESTED_STATES ( 3U )
#endif /* MAX_NESTED_STATES */

#define EVENT_ENUM(x) event_##x,
#define EVENT_ENUM_(x) event_##x

#define RETURN(x) return_##x
#define EVENT(x) EVENT_ENUM_(x)

#define STATE(x) State_##x
#define DEFINE_STATE(x) static state_ret_t STATE(x) ( state_t * this, event_t s )

#define EVENT_STR_(x) #x
#define EVENT_LOOKUP_(x) [EVENT_ENUM_(x)] = EVENT_STR_(x),

#define GENERATE_EVENTS( EVNT ) \
    enum Event \
    { \
        DEFAULT_EVENTS(EVENT_ENUM) \
        EVNT( EVENT_ENUM ) \
        EVENT_ENUM( EventCount ) \
    }

#define GENERATE_EVENT_STRINGS( EVNT ) \
    static const char *event_str[] = \
    { \
        DEFAULT_EVENTS( EVENT_LOOKUP_ ) \
        EVNT( EVENT_LOOKUP_ ) \
        EVENT_LOOKUP_( EventCount ) \
    } \

#define STATE_DEBUG( x ) \
    if( (uint32_t)x > 0U ) \
    { \
        printf("%s -> %s Event\n", __func__, event_str[x] ); \
    }

#define PARENT( X, parent_state ) ((X)->state = parent_state, RETURN( Unhandled ) )
#define TRANSITION( X, new_state ) ((X)->state = new_state, RETURN( Transition ))
#define HANDLED(X) RETURN ( Handled )
#define NO_PARENT(X) ((X)->state = NULL,RETURN( Unhandled ))

typedef uint32_t event_t;

typedef enum
{
    #define RETURN_CODE(x) RETURN(x),
        STATE_RETURN_CODES
    #undef RETURN_CODE
}
state_ret_t;

/* Forward declaration so that function pointer with state can return itself */
typedef struct state_t state_t;

/* Function pointer that holds the state to execute */
typedef state_ret_t ( *state_func_t ) ( state_t * this, event_t s );

struct state_t
{
    state_func_t state;
};

typedef struct
{
    state_t * state;
    event_t event;
}
state_event_t;

extern void STATEMACHINE_Init( state_t * state, state_ret_t (*initial_state) ( state_t * this, event_t s ) );
extern void STATEMACHINE_Dispatch( state_t * state, event_t s );

#ifdef UNIT_TESTS
#include "fifo_base.h"
#include "state_history.h"
extern fifo_base_t * STATE_GetHistory ( void );
#endif

#endif /* STATE_H_ */

