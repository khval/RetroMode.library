
extern struct ExecIFace		*IExec;
extern struct Library			*DOSBase;
extern struct DOSIFace		*IDOS;
extern struct Library			*GraphicsBase;
extern struct GraphicsIFace	*IGraphics;

struct RetroLibrary
{
	struct Library libNode;
	BPTR segList;

	/* If you need more data fields, add them here */
};

