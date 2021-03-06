#ifndef RWTE_SELECTION_H
#define RWTE_SELECTION_H

#include <memory>
#include <time.h>

enum selection_mode
{
	SEL_IDLE = 0,
	SEL_EMPTY = 1,
	SEL_READY = 2
};

enum selection_type
{
	SEL_REGULAR = 1,
	SEL_RECTANGULAR = 2
};

enum selection_snap
{
	SNAP_WORD = 1,
	SNAP_LINE = 2
};

struct Selection
{
    Selection();

    void clear();
    bool empty() const;
    bool selected(int col, int row) const;

	int mode = SEL_IDLE;
	int type = 0;
	int snap = 0;

	// Selection variables:
	// nb – normalized coordinates of the beginning of the selection
	// ne – normalized coordinates of the end of the selection
	// ob – original coordinates of the beginning of the selection
	// oe – original coordinates of the end of the selection
	struct {
		int col, row;
	} nb, ne, ob, oe;

	std::shared_ptr<char> primary;
    std::shared_ptr<char> clipboard;

	bool alt = false;
	struct timespec tclick1 = {0};
	struct timespec tclick2 = {0};
};

#endif // RWTE_SELECTION_H
