#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_coder
{
    int             id;
    pthread_t       thread;
    long long       last_time_for_compile;
    long            time_left_compile;
    int             compile;
    struct s_data   *data;
    struct s_coder  *next;
    long long       ticket;
}   t_coders;

typedef struct s_priority
{
    int         number_id;
    long long   priority;
}   t_priority;

typedef struct s_dongle
{
    int             id;
    int             *coder;
    int             is_used;
    long            time_compile;
    t_priority      priority;
    pthread_mutex_t *mutex;
    struct s_data   *data;
    struct s_dongle *next;
}   t_dongle;

typedef struct s_data
{
    int             number_of_coders;
    long long       time_to_burnout;
    long long       time_to_compile;
    long long       time_to_debug;
    int             burn_out_time;
    int             time_to_refactor;
    int             number_of_compiles_required;
    int             dongle_cooldown;
    long long       ticket_place;
    char            *scheduler;
    long long       start_time;
    pthread_cond_t  cond;
    t_coders        *coder;
    pthread_mutex_t write_mutex;
    t_dongle        *dongle;
}   t_data;

t_coders    *ft_lstnew(int id);
t_coders    *ft_lstlast(t_coders *lst);
void        lstadd_back(t_coders **lst, t_coders *new_node);
t_coders    *all_coders(int number_of_coders);
void        free_coders(t_coders *lst);

t_dongle    *dongle_ft_lstnew(int id, int number_coders);
t_dongle    *dongle_ft_lstlast(t_dongle *lst);
void        dongle_add_back(t_dongle **lst, t_dongle *new_node);
t_dongle    *all_dongles(int number_of_coders);
void        free_dongles(t_dongle *lst);
t_data      parser(int argc, char **argv);

void        *coder_routine(void *arg);
void        start_simulation(t_data *data);
long long   timestamp(void);
void        log_action(t_data *data, int coder_id, char *msg);
t_dongle    *get_dongle_id(t_dongle *dongles, int id);
void        manage_dongle(t_dongle *dongle, t_data *data, int id, char* msg);
int 		ft_strcmp(char *str, char *str2);
void    	*monitoring(void *arg);
int 		simulation_is_end(t_data *data);
void	*taking_dongle_and_compile(t_data *data, t_coders *coder,
	t_dongle *left_dongle, t_dongle *right_dongle);

int			my_turn_fifo(t_coders *coder);
int			my_turn_edf(t_coders *coder);

void	*is_fonction_fifo(t_data *data, t_coders *coder);
void	*is_fonction_edf(t_data *data, t_coders *coder);

int ft_strcmp(char *str, char *str2);

#endif
