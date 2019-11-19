#include "MultiTasker.h"

MultiTasker* MultiTasker::Tasker = new MultiTasker();
MultiTasker* MultiTasker::SetMultiTasker() {
	if (Tasker == nullptr) {
		//there is no instance of MultiTasker, so create one

		Tasker = new MultiTasker();
	}
	return Tasker;
}