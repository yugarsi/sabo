struct global_buffer {
    void *address;
    unsigned int length;
    struct global_buffer *next;
};


struct global_buffer *head = NULL;
struct global_buffer *curr = NULL;

struct global_buffer* create_list(void *address, unsigned int length)
{
    struct global_buffer *ptr = (struct global_buffer*)malloc(sizeof(struct global_buffer));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }
    ptr->address = address;
    ptr->length  = length;
    ptr->next = NULL;
    head = curr = ptr;
    return ptr;
}

struct global_buffer* add_to_list(void *address, unsigned int length )
{
    if(NULL == head)
    {
        return (create_list(val));
    }

    struct global_buffer *ptr = (struct global_buffer*)malloc(sizeof(struct global_buffer));
    if(NULL == ptr)
    {
        printf("\n Node creation failed \n");
        return NULL;
    }

    ptr->address = address;
    ptr->length  = length;
    ptr->next = NULL;

    curr->next = ptr;
    curr = ptr;
    return ptr;
}


