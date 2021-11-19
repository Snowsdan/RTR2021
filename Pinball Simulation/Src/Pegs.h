#include "RTRObject.h"

class RTRPegs {
public: 
	RTRPegs();
	void MovePegs();

private:
	RTRCube* leftPeg;
	RTRCube* rightPeg;
};