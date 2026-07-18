#include "gpio/exti.hpp"

#include "common/registers.hpp"

#include "rcc/rcc.hpp"

namespace exti {

// Allocate the static array mapping
std::array<ExternalInterrupt*, 16> ExternalInterrupt::active_instances {
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
};

void ExternalInterrupt::setCallback(callback::Callback func) {
	callbacks_[static_cast<std::size_t>(edge_)] = func;
}

void ExternalInterrupt::handleISR() {
	if(reg::readBit(EXTI->PR, pinNumber_)) {
		EXTI->PR = reg::singleBitMask(pinNumber_);
		callbacks_[static_cast<std::size_t>(edge_)]();
	}
}

void ExternalInterrupt::init() {
	rcc::enableClock_SYSCFG();

	reg::setBitField<4>(SYSCFG->EXTICR[Pin::pinNumber_ / 4], (Pin::pinNumber_ % 4) * 4, gpio::GPIO_syscfg_map(Pin::port_));

	reg::setBit(EXTI->IMR, Pin::pinNumber_);

	if(edge_ == Edge::RISING || edge_ == Edge::BOTH) {
		reg::setBit(EXTI->RTSR, Pin::pinNumber_);
	}
	if(edge_ == Edge::FALLING || edge_ == Edge::BOTH) {
		reg::setBit(EXTI->FTSR, Pin::pinNumber_);
	}

	enableNVIC();
}


void ExternalInterrupt::enableNVIC() {
	IRQn_Type irq;

	if(Pin::pinNumber_ == 0) {
		irq = EXTI0_IRQn;
	}
	else if(Pin::pinNumber_ == 1) {
		irq = EXTI1_IRQn;
	}
	else if(Pin::pinNumber_ == 2) {
		irq = EXTI2_IRQn;
	}
	else if(Pin::pinNumber_ == 3) {
		irq = EXTI3_IRQn;
	}
	else if(Pin::pinNumber_ == 4) {
		irq = EXTI4_IRQn;
	}
	else if(Pin::pinNumber_ >= 5 && Pin::pinNumber_ <= 9) {
		irq = EXTI9_5_IRQn;
	}
	else if(Pin::pinNumber_ >= 10 && Pin::pinNumber_ <= 15) {
		irq = EXTI15_10_IRQn;
	}
	else return;

	NVIC_EnableIRQ(irq);      // Auto-enable
}

void ExternalInterrupt::register_instances() {
	active_instances[Pin::pinNumber_] = this;
}

} // namespace exti

extern "C" {

void EXTI0_IRQHandler(void) {
	if(exti::ExternalInterrupt::active_instances[0]) {
		exti::ExternalInterrupt::active_instances[0]->handleISR();
	}
}

void EXTI1_IRQHandler(void) {
	if(exti::ExternalInterrupt::active_instances[1]) {
		exti::ExternalInterrupt::active_instances[1]->handleISR();
	}
}

void EXTI2_IRQHandler(void) {
	if(exti::ExternalInterrupt::active_instances[2]) {
		exti::ExternalInterrupt::active_instances[2]->handleISR();
	}
}

void EXTI3_IRQHandler(void) {
	if(exti::ExternalInterrupt::active_instances[3]) {
		exti::ExternalInterrupt::active_instances[3]->handleISR();
	}
}

void EXTI4_IRQHandler(void) {
	if(exti::ExternalInterrupt::active_instances[4]) {
		exti::ExternalInterrupt::active_instances[4]->handleISR();
	}
}

void EXTI9_5_IRQHandler(void) {
	if(exti::ExternalInterrupt::active_instances[5]) {
		exti::ExternalInterrupt::active_instances[5]->handleISR();
	}
	if(exti::ExternalInterrupt::active_instances[6]) {
		exti::ExternalInterrupt::active_instances[6]->handleISR();
	}
	if(exti::ExternalInterrupt::active_instances[7]) {
		exti::ExternalInterrupt::active_instances[7]->handleISR();
	}
	if(exti::ExternalInterrupt::active_instances[8]) {
		exti::ExternalInterrupt::active_instances[8]->handleISR();
	}
	if(exti::ExternalInterrupt::active_instances[9]) {
		exti::ExternalInterrupt::active_instances[9]->handleISR();
	}
}

void EXTI15_10_IRQHandler(void) {
	if(exti::ExternalInterrupt::active_instances[10]) {
		exti::ExternalInterrupt::active_instances[10]->handleISR();
	}
	if(exti::ExternalInterrupt::active_instances[11]) {
		exti::ExternalInterrupt::active_instances[11]->handleISR();
	}
	if(exti::ExternalInterrupt::active_instances[12]) {
		exti::ExternalInterrupt::active_instances[12]->handleISR();
	}
	if(exti::ExternalInterrupt::active_instances[13]) {
		exti::ExternalInterrupt::active_instances[13]->handleISR();
	}
	if(exti::ExternalInterrupt::active_instances[14]) {
		exti::ExternalInterrupt::active_instances[14]->handleISR();
	}
	if(exti::ExternalInterrupt::active_instances[15]) {
		exti::ExternalInterrupt::active_instances[15]->handleISR();
	}
}

}

