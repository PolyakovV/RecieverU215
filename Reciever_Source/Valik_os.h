#define MAX_PROCESSES 6
#define MAX_SUBSCRIBERS 30

#define MAX_NAME_LEN 20
struct pcb{
        int pid;  /*  process ID*/
        int prio; /* priority of process */
        int attached; /* 1 if attached, if no - 0 */
        int *function; /* function address  */
        char name[MAX_NAME_LEN]; /* process name */
};

static  pcb processlist[MAX_PROCESSES]; // create list

struct subscriber_str{
                      int pid; /*  process ID*/
                      int *function; /* function address  */
                      char  subscr_name[MAX_NAME_LEN]; /* process name */
                      char subscr_event[MAX_NAME_LEN]; /* name of trigger event */
                      int attached; /* 1 if attached, if no - 0 */
};


static  subscriber_str subscribers_list[MAX_SUBSCRIBERS]; // create list
///////////External Function//////////////

extern int process_attach(char *name, int prio, void *function);
extern int process_detach(int pid);
extern int process_detach_by_name (char *name);
extern int scheduler();

/////////////Mediator///////////////////////
extern int process_subscribe (char *subscr_event, char *subscr_name, int *function);
extern int process_unsubscribe_by_name (char *subscr_name);
extern int process_unsubscribe_by_event (char *subscr_event);
extern int process_trigger (char *subscr_event);
