#define MAX 10

typedef struct queue_t
{
	
	int intArray[MAX];
	int front;
	int rear ;
	int count ;
	int empty;


} struct_queue_t;

void insertElement(struct_queue_t * queue, int data);
int removeElement(struct_queue_t * queue);
int isFull(struct_queue_t * queue);
int isEmpty(struct_queue_t * queue);
int peek(struct_queue_t * queue);