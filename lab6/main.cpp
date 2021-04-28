#include "ThisThread.h"
#include "Thread.h"
#include "Timeout.h"
#include "mbed.h"

DigitalOut led1(LED1);
InterruptIn button(USER_BUTTON);
EventQueue queue(32 * EVENTS_EVENT_SIZE); 
Timeout press_threshold;
Thread t;

void button_release_detecting()
{
    button.enable_irq();
    queue.call(printf, "Timer ends\n");
}

void button_pressed(void) {
    // Toggle LED
    led1 = !led1;
    osThreadId_t fall_t_id = ThisThread::get_id(); 
    button.disable_irq();
    queue.call(printf, "fall_handler in context %p\n", fall_t_id);
    press_threshold.attach(&button_release_detecting, 3.0);
    queue.call(printf, "Start timer... \n");
}

void button_released(void) {
    // Toggle LED
    led1 = !led1;
    printf("rise_handler in context %p\n", ThisThread::get_id());
}

int main() {
    // Start the event queue
    t.start(callback(&queue, &EventQueue::dispatch_forever)); 
    printf("Starting in context %p\r\n", ThisThread::get_id()); 
    // The 'fall' handler will execute in the context of thread 't'
    button.fall(button_pressed);
    // The 'rise' handler will execute in IRQ context 
    button.rise(queue.event(button_released));
}

