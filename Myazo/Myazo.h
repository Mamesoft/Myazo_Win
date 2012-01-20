#include "ShareHeaders.h"
#include "Window.h"
#include "PNGFile.h"

class Myazo
{
private:
	HINSTANCE Instance;
	bool UploadAsPrivate,UtilityMode;
	PNGFile ImageEncoder;
	Window* MainWindow,* LayerWindow;
	int OffsetX,OffsetY;

	Myazo(const Myazo&);
	bool InitWindow(void);
	bool Upload(void);

public:
	Myazo(void);
	~Myazo(void);
	bool Init(void);
	void EnterMessageLoop(void);

};