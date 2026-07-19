#include "gpio/exti.hpp"
#include "common/registers.hpp"
#include "rcc/rcc.hpp"

namespace exti {

std::array<ExternalInterrupt*, 16> ExternalInterrupt::active_instances{nullptr};

void ExternalInterrupt::setCallback(Edge triggerEdge, callback::Callback func) {
    if (triggerEdge == Edge::BOTH) {
        callbacks_[0] = func;
        callbacks_[1] = func;
    } else {
        callbacks_[static_cast<std::size_t>(triggerEdge)] = func;
    }
}

void ExternalInterrupt::handleISR() const {
    if (reg::readBit(EXTI->PR, pinNumber_)) {
        // Write 1 to clear pending flag
        EXTI->PR = reg::singleBitMask(pinNumber_);

        bool pin_is_low = !reg::readBit(port_->IDR, pinNumber_);

        if (pin_is_low && callbacks_[1]) {
            callbacks_[1]();
        } else if (!pin_is_low && callbacks_[0]) {
            callbacks_[0]();
        }
    }
}

void ExternalInterrupt::init() {
    rcc::enableClock_SYSCFG();

    reg::setBitField<4>(SYSCFG->EXTICR[pinNumber_ / 4], (pinNumber_ % 4) * 4, gpio::mapSyscfgGpio(port_));
    reg::setBit(EXTI->IMR, pinNumber_);

    if (edge_ == Edge::RISING || edge_ == Edge::BOTH) {
        reg::setBit(EXTI->RTSR, pinNumber_);
    }
    if (edge_ == Edge::FALLING || edge_ == Edge::BOTH) {
        reg::setBit(EXTI->FTSR, pinNumber_);
    }

    enableNVIC();
}

void ExternalInterrupt::enableNVIC() const {
    IRQn_Type irq;

    if      (pinNumber_ == 0) irq = EXTI0_IRQn;
    else if (pinNumber_ == 1) irq = EXTI1_IRQn;
    else if (pinNumber_ == 2) irq = EXTI2_IRQn;
    else if (pinNumber_ == 3) irq = EXTI3_IRQn;
    else if (pinNumber_ == 4) irq = EXTI4_IRQn;
    else if (pinNumber_ >= 5 && pinNumber_ <= 9)   irq = EXTI9_5_IRQn;
    else if (pinNumber_ >= 10 && pinNumber_ <= 15) irq = EXTI15_10_IRQn;
    else return;

    NVIC_EnableIRQ(irq);
}

void ExternalInterrupt::registerInstance() {
    active_instances[pinNumber_] = this;
}

} // namespace exti

extern "C" {
void EXTI0_IRQHandler(void)    { if(exti::ExternalInterrupt::active_instances[0])  exti::ExternalInterrupt::active_instances[0]->handleISR(); }
void EXTI1_IRQHandler(void)    { if(exti::ExternalInterrupt::active_instances[1])  exti::ExternalInterrupt::active_instances[1]->handleISR(); }
void EXTI2_IRQHandler(void)    { if(exti::ExternalInterrupt::active_instances[2])  exti::ExternalInterrupt::active_instances[2]->handleISR(); }
void EXTI3_IRQHandler(void)    { if(exti::ExternalInterrupt::active_instances[3])  exti::ExternalInterrupt::active_instances[3]->handleISR(); }
void EXTI4_IRQHandler(void)    { if(exti::ExternalInterrupt::active_instances[4])  exti::ExternalInterrupt::active_instances[4]->handleISR(); }

void EXTI9_5_IRQHandler(void) {
    for (int i = 5; i <= 9; ++i) {
        if (exti::ExternalInterrupt::active_instances[i]) {
            exti::ExternalInterrupt::active_instances[i]->handleISR();
        }
    }
}

void EXTI15_10_IRQHandler(void) {
    for (int i = 10; i <= 15; ++i) {
        if (exti::ExternalInterrupt::active_instances[i]) {
            exti::ExternalInterrupt::active_instances[i]->handleISR();
        }
    }
}
} // extern "C"
