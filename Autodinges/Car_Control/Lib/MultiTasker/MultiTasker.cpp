#include "MultiTasker.h"

MultiTasker MultiTasker::taskerval;
MultiTasker* MultiTasker::Tasker = nullptr;
MultiTasker* MultiTasker::SetMultiTasker() {
	if (Tasker == nullptr) {
		//there is no instance of MultiTasker, so create one
		Tasker = &taskerval;
	}
	return Tasker;
}