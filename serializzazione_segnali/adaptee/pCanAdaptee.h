#ifndef PCAN_ADAPTEE
#define PCAN_ADAPTEE

#include "canAdapter.h"
#include <windows.h>
#include "PCANBasic.h"
#include "logger.h"
#include <array>

class pCanAdaptee final : public canAdapter {
public:
	/*costructor, destructor, copy, move*/
	pCanAdaptee();
	~pCanAdaptee() = default; //final, no virtual
	pCanAdaptee(const pCanAdaptee&) = delete;
	pCanAdaptee(pCanAdaptee&&) = delete;
	pCanAdaptee& operator = (const pCanAdaptee&) = delete;
	pCanAdaptee& operator = (pCanAdaptee&&) = delete;


private:
	/*private typedefs*/
	struct pCanStatus {
		char statusString[256];
		TPCANStatus status;
	};

protected:
	/*overrided methods*/
	void connectToDevice() override; //thow a runtime error if cannot find any pCan device
	void disconnectFromDevice() noexcept override;
	void readFromDevice() noexcept override;
	void writeToDevice() noexcept override;

private:
	/*private members*/
	TPCANHandle deviceHandler = 0;
	std::array<uint16_t, 16> pCanUsbInterfaces{PCAN_USBBUS1,  PCAN_USBBUS2,  PCAN_USBBUS3,  PCAN_USBBUS4,
											   PCAN_USBBUS5,  PCAN_USBBUS6,  PCAN_USBBUS7,  PCAN_USBBUS8,
											   PCAN_USBBUS9,  PCAN_USBBUS10, PCAN_USBBUS11, PCAN_USBBUS12,
											   PCAN_USBBUS13, PCAN_USBBUS14, PCAN_USBBUS15, PCAN_USBBUS16};
private:
	/* private methods*/
	pCanStatus getStatus() const noexcept;


};
#endif