#define WM_DV_TIMECHANGE	(WM_USER+201)

class CDShowException: public CException {
public:
	CDShowException(BOOL b_AutoDelete, int cause, LPCSTR message);
	int m_cause;
	enum {none = 0, deviceNotFound, error};
	CString m_message;
	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL);
protected:
};

void ThrowDShowException(int cause, LPCSTR message);

class CFrameHandler {
public:
	virtual void HandleFrame(REFERENCE_TIME duration, BYTE *data, int len) = 0;
};

class CFrameSource {
public:
	virtual void GetMediaType(CMediaType *type) = 0;
	virtual void Run(CFrameHandler *handler) = 0;
	virtual void Stop() = 0;
};

class CFilterGraph {
public:
	ICaptureGraphBuilder2 *m_GB;
	IGraphBuilder	*m_FG;
	IMediaControl *m_MC;
	IMediaSeeking *m_MS;
	IMediaEventEx *m_ME;
	CFilterGraph();
	virtual ~CFilterGraph();
};

class CInputGraph:public CFilterGraph, CFrameSource {
public:
	CFrameHandler *m_handler;

	class CInputFilter: public CBaseFilter {
	public:
		CInputGraph *m_graph;

		CInputFilter(CInputGraph *graph);
		~CInputFilter();
		int GetPinCount();
		CBasePin *GetPin(int n);
		CCritSec m_cs;

		class CInputPin : public CBaseInputPin {
		public:
			CInputPin(CInputFilter *pFilter, CCritSec *cs, HRESULT *phr);
			HRESULT CheckMediaType(const CMediaType *pmt);
			STDMETHODIMP Receive(IMediaSample *pSample);
    		STDMETHODIMP EndOfStream();

		} *m_input;
	} *m_inputFilter;

	CInputGraph();
	void GetMediaType(CMediaType *type);
	void Run(CFrameHandler *handler);
	void Stop();
	~CInputGraph();
};

class COutputGraph:public CFilterGraph, public CFrameHandler {
public:
	CMediaType m_type;
	REFERENCE_TIME m_time;

	int m_queue;

	class COutputFilter : public CBaseFilter {
	public:
		COutputGraph *m_graph;

		COutputFilter(COutputGraph *graph);
		~COutputFilter();
		int GetPinCount();
		CBasePin *GetPin(int n);
		CCritSec m_cs;

		class COutputPin : public CBaseOutputPin {
		public:
			COutputQueue *m_queue;
			COutputPin(COutputFilter *pFilter, CCritSec *cs, HRESULT *phr);
			HRESULT GetMediaType(int iPosition, CMediaType *pmt);
			HRESULT CheckMediaType(const CMediaType *pmt);
			HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *ppropInputRequest);
			HRESULT Deliver(IMediaSample *pSample);
			HRESULT DeliverEndOfStream();
			HRESULT Active();
			HRESULT Inactive();
		} *m_output;
	} *m_outputFilter;	
	
	COutputGraph(CMediaType *type, int queue = 0);
	~COutputGraph();
	void HandleFrame(REFERENCE_TIME duration, BYTE *data, int len);
};

class CAVIReader:public CInputGraph {
public:
	CAVIReader(LPCSTR filename);
};

class CAVIJoiner:public CFrameSource, CFrameHandler {
public:
	CFrameHandler *m_joinHandler;
	CArray<CString, CString&> m_filenames;
	CAVIReader *m_reader;
	CEvent m_ev;
	CWinThread *m_thread;
	int m_current;
	bool m_stopping;

	CAVIJoiner(LPCSTR filenames);
	~CAVIJoiner();
	void GetMediaType(CMediaType *type);
	void Run(CFrameHandler *handler);
	void Stop();
	void HandleFrame(REFERENCE_TIME duration, BYTE *data, int len);
	void JoinerThread();
};

class CDVControl {
	IAMExtTransport *m_ET;
public:
	CDVControl();
	~CDVControl();
	void CtrlAttach(IUnknown *pDev);
	void CtrlStop();
	void CtrlPlay();
	void CtrlPause();
	void CtrlRecord();
	void CtrlRecPause();
};

class CDVInput:public CInputGraph, public CDVControl {
public:
	IAMDroppedFrames * m_DF;
	CDVInput(LPCSTR vsrc); 
	~CDVInput(); 
	long GetDroppedFrames();
};

class CDVOutput:public COutputGraph, public CDVControl {
public:
	CDVOutput(LPCSTR vdst, CMediaType *type);
	~CDVOutput();
};

class CAVIWriter:public COutputGraph {
public:
	CString m_tmpfile, m_filename, m_dtformat;
	int m_ndigits;
	time_t m_dvtime;

	CAVIWriter(LPCSTR filename, LPCSTR dtformat, int ndigits, time_t tim, bool type2AVI, CMediaType *type);
	~CAVIWriter();
};

class CMonitor:public COutputGraph {
public:
	HWND m_hWnd;
	IVideoWindow *m_VW;
	IMediaSample *m_sample;
	CWinThread *m_thread;
	CEvent m_ev;

	CMonitor(HWND hWnd, CMediaType *type);
	~CMonitor();

	void Resize();
	void HandleFrame(REFERENCE_TIME duration, BYTE *data, int len);
	void MonitoringThread();
};


class CDVQueue {
	struct Buffer {
		REFERENCE_TIME duration;
		int len;
		BYTE data[1];
	};
public:
	CEvent m_evPut, m_evGet;
	CCritSec m_cs;
	BYTE *m_buffers;
	Buffer ** m_queue;
	int m_dataSize, m_queueSize, m_head, m_tail, m_load;
	bool m_end;
	CDVQueue(int queueSize, int dataSize);
	~CDVQueue();
	void Put(REFERENCE_TIME duration, BYTE *data, int len);
	bool Get(REFERENCE_TIME *duration, BYTE **data, int *len);
};

class CDV:public CStatic, CFrameHandler  {
public:
	enum {Idle, RecordPaused, Recording, CapturePaused, Capturing, Finished} m_state;
	bool m_type2AVI;
	int m_discontinuityTreshold;
	int m_maxAVIFrames;
	int m_everyNth;
	bool m_recordPreview;
	bool m_DVctrl;

	CDV();
	~CDV();

	int GetState();
	int GetDropped();
	int GetQueueLoad();
	long GetCounter();
	REFERENCE_TIME GetTime();
	CString GetCaptureFilename();

	void Destroy();

	void BuildCapturing(LPCSTR vsrc);
	void StartCapturing(LPCSTR filename, LPCSTR dtformat, int ndigits, REFERENCE_TIME captureTime = 0);
	void StopCapturing();
	void BuildRecording(LPCSTR filenames, LPCSTR vdst);
	void StartRecording();
	void StopRecording();

	void RecordingThread();
	void CapturingThread();
protected:
	CAVIJoiner *m_aviJoiner;
	CAVIWriter *m_aviWriter;
	CDVInput * m_dvInput;
	CDVOutput * m_dvOutput;
	CMonitor *m_monitor;
	CDVQueue * m_queue;

	CCritSec m_cs;

	CWinThread *m_thread;
	CString m_captureFilename, m_dtformat;
	int m_ndigits;
	long m_dropped;
	long m_counter;
	REFERENCE_TIME m_time;
	REFERENCE_TIME m_captureTime;

	void HandleFrame(REFERENCE_TIME duration, BYTE *data, int len);

	//{{AFX_MSG(CDV)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

void GetVideoSrcList(CArray<CString,CString&> &);
void GetVideoDstList(CArray<CString,CString&> &);

CString FormatTime(LPCSTR format, time_t tim);
