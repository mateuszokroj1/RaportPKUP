#include <QObject>

class WindowController : public QObject
{
	Q_OBJECT
public:
	WindowController() = default;

	//Q_PROPERTY(QString author READ isNightMode WRITE setIsNightMode NOTIFY isNightModeChanged)

	
};