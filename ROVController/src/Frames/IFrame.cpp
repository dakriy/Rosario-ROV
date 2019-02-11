#include "IFrame.h"
#include "../Core/GlobalContext.h"

Frames::IFrame::IFrame()
{
	window_ = GlobalContext::get_window();
}

void Frames::IFrame::show()
{
	hidden = false;
}

void Frames::IFrame::hide()
{
	hidden = true;
}

void Frames::IFrame::pause()
{
	paused = true;
}

void Frames::IFrame::unpause()
{
	paused = false;
}

bool Frames::IFrame::isHidden() const
{
	return hidden;
}

bool Frames::IFrame::isPaused() const
{
	return paused;
}

bool Frames::operator==(const IFrame& lhs, const IFrame& rhs)
{
	return lhs.get_type() == rhs.get_type();
}

bool Frames::operator!=(const IFrame& lhs, const IFrame& rhs)
{
	return !(lhs == rhs);
}
