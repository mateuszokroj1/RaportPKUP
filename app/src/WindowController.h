#pragma once

#include <QObject>

#include "Application.hpp"

namespace RaportPKUP
{
class WindowController : public QObject
{
	Q_OBJECT

  public:
	WindowController(std::weak_ptr<Application> app);

	// Q_PROPERTY(QString author READ isNightMode WRITE setIsNightMode NOTIFY
	// isNightModeChanged)

  private:
	std::weak_ptr<Application> _application;
};
} // namespace RaportPKUP
