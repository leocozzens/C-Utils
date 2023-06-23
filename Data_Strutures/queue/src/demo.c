#include <queue.h>
#include <time.h>

#define Q_COUNT 10

int main(void) {
    int out;

    srand(time(NULL));
    Queue q[Q_COUNT]; 
    for(int i = 0; i < Q_COUNT; i++) make_queue(&q[i]);

    for(int qIndex = 0; qIndex < Q_COUNT; qIndex++) {
            for(int i = 0; i < 2; i++) {
            printf("\nQueue index: %d\nTry: %d\n", qIndex + 1, i + 1);
            for(int j = 10; j <= 100; j += 10) {
                enqueue(&q[qIndex], rand() % (j + 1));
            }

            while(dequeue(&q[qIndex], &out)) {
                printf("out = %d\n", out);
            }
            if(q[qIndex].head == NULL && q[qIndex].tail == NULL) printf("Empty!\n");
        }
    }
    return 0;
}