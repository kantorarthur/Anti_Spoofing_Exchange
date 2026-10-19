#include "order_book.h"
#include <iostream>

int main()
{
    Order order;
    order.order_id = 3;
    std::cout << order.order_id << " " << sizeof(Order);
}
