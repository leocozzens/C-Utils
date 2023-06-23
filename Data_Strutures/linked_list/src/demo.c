#include <linked_list.h>

int main(void) {
    Node *head = NULL;
    Node *tmp;

    insert_first(&head, 0);
    tmp = head;
    for(int i = 1; i <= 25; i++) {
        //insert_first(&head, i);
        insert_after(tmp, create_node(i));
        tmp = tmp->next;
    }

    insert_after(find_node(&head, 12), create_node(99));
    delete_after(find_prev(&head, 12));

    print_list(head);

    free(delete_list(&head));
    return 0;
}