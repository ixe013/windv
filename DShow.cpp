#include "stdafx.h"
#include <process.h>
#include "DShow.h"
#include "DV.h"

/////////////////////////////////////////////////////////////////////////////

inline void CHECK_HR(HRESULT hr, LPCSTR message = "Error", int cause = CDShowException::error) 
{
	if (hr != S_OK) ThrowDShowException(cause, message);
}

static void SetDVDecoding(IGraphBuilder *pFG, int full = 0)
{
	IBaseFilter *pDVFilt;
	IIPDVDec *pDVDec;
	HRESULT hr;

	hr = pFG->FindFilterByName(L"DV Video Decoder", &pDVFilt);
	if (hr == S_OK) {
		hr = pDVFilt->QueryInterface(IID_IIPDVDec, (void**)&pDVDec);
		if (hr == S_OK) {
			pDVDec->put_IPDisplay(full ? DVDECODERRESOLUTION_720x480 : DVDECODERRESOLUTION_360x240);
		}
		pDVFilt->Release();
	}
}


static void EnumVideoDevices(BOOL record, LPCSTR device, CArray<CString,CString &> &list, IBaseFilter ** pfilter)
{
	HRESULT hr;

	if (pfilter) *pfilter = NULL;

    ICreateDevEnum *pCreateDevEnum;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC,
			  IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    CHECK_HR(hr);
    IEnumMoniker *pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
								&pEm, 0);
    pCreateDevEnum->Release();
	CHECK_HR(hr, "No video device found", CDShowException::deviceNotFound);
    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
    while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
    {
	    IPropertyBag *pBag;
	    hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
	    if(SUCCEEDED(hr)) {
			VARIANT varName;
			varName.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &varName, NULL);
			CHECK_HR(hr);
			pBag->Release();
			CString tmp = varName.bstrVal;
			list.Add(tmp);
			if (device && pfilter && tmp == device && !*pfilter) {
				hr = pM->BindToObject(NULL, NULL, IID_IBaseFilter, (void **)pfilter);
				CHECK_HR(hr);
			}
			SysFreeString(varName.bstrVal);
		}
		pM->Release();
    }
    pEm->Release();

	if (pfilter && !*pfilter) ThrowDShowException(CDShowException::deviceNotFound, "Device not found");
}

void GetVideoSrcList(CArray<CString,CString &> &list)
{
	EnumVideoDevices(FALSE, NULL, list, NULL);
}


void GetVideoDstList(CArray<CString,CString &> &list)
{
	EnumVideoDevices(TRUE, NULL, list, NULL);
}

static CString GetCaptureFilename(LPCSTR base, LPCSTR dtformat, int ndigits, time_t tim)
{
	CString basdattim = base;
	if (tim <= 0) tim = time(NULL);
	CString tmp = FormatTime(dtformat, tim);
	if (!tmp.IsEmpty()) {
		basdattim += ".";
		basdattim += tmp;
	}
	tmp = basdattim;
	int bassiz = 0;
	int i = basdattim.GetLength();
	while (i) {
		i--;
		switch (basdattim[i]) {
		case '\\':
		case '/':
		case ':':
			goto endwhile;
		}
		bassiz++;
	}
endwhile:

	tmp += ".*.avi";

	int max = -1;
	int maxn = ndigits;

	CFileFind ff;
	BOOL found = ff.FindFile(tmp);
	while (found) {
		found = ff.FindNextFile();
		tmp = ff.GetFileName();
		int l = tmp.GetLength() - bassiz - 5;
		if (l > 0 && l >= maxn) {
			tmp = tmp.Mid(bassiz+1,l);
			int j = 0;
			for(i=0; i<l; i++) {
				if (!isdigit(tmp[i])) goto next;
				j = j*10 + (tmp[i]-'0');
			}
			if (l>maxn) {
				maxn = l; max = j;
			}
			else if (j > max) max = j;
		}
next:;
	}
	ff.Close();

	tmp = basdattim;

	CString num;
	num.Format(".%0*d", maxn, max+1);

	if (!maxn) {
		if (ff.FindFile(basdattim+".avi")) maxn++;

		ff.Close();

	}

	if (maxn) tmp += num;

	return tmp + ".avi";
}

/////////////////////////////////////////////////////////////////////////////

CDShowException::CDShowException(BOOL b_AutoDelete, int cause, LPCSTR message)
: CException(b_AutoDelete)
{
	m_cause = cause;
	m_message = message;
}

BOOL CDShowException::GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
	lstrcpyn(lpszError, m_message, nMaxError);
	if (pnHelpContext) *pnHelpContext = 0;
	return TRUE;
}

void ThrowDShowException(int cause, LPCSTR message)
{
	THROW(new CDShowException(TRUE, cause, message));
}

/////////////////////////////////////////////////////////////////////////////

CFilterGraph::CFilterGraph()
{
	HRESULT hr;
	hr = CoCreateInstance((REFCLSID)CLSID_CaptureGraphBuilder2,
				  NULL, CLSCTX_INPROC, (REFIID)IID_ICaptureGraphBuilder2,
				  (void **)&m_GB);
	CHECK_HR(hr, "Can't create CaptureGraphBuilder");

	hr = CoCreateInstance((REFCLSID)CLSID_FilterGraph,
				  NULL, CLSCTX_INPROC, (REFIID)IID_IGraphBuilder,
				  (void **)&m_FG);
	CHECK_HR(hr, "Can't create FilterGraph");

	hr = m_GB->SetFiltergraph(m_FG);
	CHECK_HR(hr);

	hr = m_FG->QueryInterface(IID_IMediaControl, (void **)&m_MC);
	CHECK_HR(hr);

	hr = m_FG->QueryInterface(IID_IMediaSeeking, (void **)&m_MS);
	CHECK_HR(hr);

	hr = m_FG->QueryInterface(IID_IMediaEventEx, (void **)&m_ME);
	CHECK_HR(hr);
}

CFilterGraph::~CFilterGraph()
{
	if (m_MC) {m_MC->Stop(); m_MC->Release();}
	if (m_MS) m_MS->Release();
	if (m_ME) m_ME->Release();
	if (m_FG) m_FG->Release();
	if (m_GB) m_GB->Release();
}

/////////////////////////////////////////////////////////////////////////////

CInputGraph::CInputGraph()
: m_handler(NULL)
{
	HRESULT hr;

	m_inputFilter = new CInputFilter(this); m_inputFilter->AddRef();

	hr = m_FG->AddFilter(m_inputFilter, L"InputFilter");

}

CInputGraph::~CInputGraph()
{
	Stop();
	m_inputFilter->m_graph = NULL;
	m_inputFilter->Release();
}

void CInputGraph::Run(CFrameHandler *handler)
{
	m_handler = handler;
	m_MC->Run();
}

void CInputGraph::Stop()
{
	if (m_MC) m_MC->Stop();
	m_handler = NULL;
}

void CInputGraph::GetMediaType(CMediaType *type)
{
	AM_MEDIA_TYPE mt;
	m_inputFilter->m_input->ConnectionMediaType(&mt);
	*type = mt;
	DVINFO *dvinfo = (DVINFO *)mt.pbFormat;
	FreeMediaType(mt);
	long l = type->GetSampleSize();
	if (l < 144000) type->SetSampleSize(144000);
}


CInputGraph::CInputFilter::CInputFilter(CInputGraph *graph)
: CBaseFilter(NAME("DV Destination"), NULL, &m_cs, (REFIID)CLSID_NULL), m_graph(graph)
{
	HRESULT hr = NOERROR;
	m_input = new CInputPin(this, &m_cs, &hr);
}

CInputGraph::CInputFilter::~CInputFilter()
{
	delete m_input;
}

int CInputGraph::CInputFilter::GetPinCount()
{
	return 1;
}

CBasePin *CInputGraph::CInputFilter::GetPin(int n)
{
	return m_input;
}

CInputGraph::CInputFilter::CInputPin::CInputPin(CInputFilter *pFilter, CCritSec *cs, HRESULT *phr)
: CBaseInputPin(NAME("Input"), pFilter, cs, phr, L"Input")
{
}

HRESULT CInputGraph::CInputFilter::CInputPin::CheckMediaType(const CMediaType *pmt)
{
	if (*pmt->Type() == MEDIATYPE_Interleaved) return S_OK;
	return S_FALSE;
}

STDMETHODIMP CInputGraph::CInputFilter::CInputPin::Receive(IMediaSample *pSample)
{
	HRESULT hr = CBaseInputPin::Receive(pSample);
	if (hr != NOERROR) return hr;

	REFERENCE_TIME startTime, endTime;
	pSample->GetTime(&startTime, &endTime);
	BYTE *ptr;
	pSample->GetPointer(&ptr);
	long len = pSample->GetActualDataLength();
	((CInputFilter *)m_pFilter)->m_graph->m_handler->HandleFrame(endTime - startTime, ptr, len);
	return hr;
}

STDMETHODIMP CInputGraph::CInputFilter::CInputPin::EndOfStream()
{
	((CInputFilter *)m_pFilter)->m_graph->m_handler->HandleFrame(-1, NULL, 0);
	return NOERROR;
}

/////////////////////////////////////////////////////////////////////////////

COutputGraph::COutputGraph(CMediaType *type, int queue)
: m_time(0), m_type(*type), m_queue(queue)
{
	HRESULT hr;

	m_outputFilter = new COutputFilter(this); m_outputFilter->AddRef();

	hr = m_FG->AddFilter(m_outputFilter, L"OutputFilter");
}

struct FrameInfo {
	REFERENCE_TIME duration;
	BYTE *data;
	int len;
};


void COutputGraph::HandleFrame(REFERENCE_TIME duration, BYTE *data, int len)
{
	if (data) {
		IMediaSample *pSample = NULL;
		HRESULT hr;
		
		hr = m_outputFilter->m_output->GetDeliveryBuffer(&pSample, NULL, NULL, 0);
		CHECK_HR(hr);

		BYTE *ptr;

		pSample->GetPointer(&ptr);

		CopyMemory(ptr, data, len);
		pSample->SetActualDataLength(len);

		REFERENCE_TIME newTime = m_time + duration;
		if (duration) pSample->SetTime(&m_time, &newTime);
		m_time = newTime;
		pSample->SetSyncPoint(true);

		m_outputFilter->m_output->Deliver(pSample);

		pSample->Release();
	}
}


COutputGraph::~COutputGraph()
{
	m_outputFilter->Release();
}


COutputGraph::COutputFilter::COutputFilter(COutputGraph *graph)
: CBaseFilter(NAME("DV Source"), NULL, &m_cs, (REFIID)CLSID_NULL), m_graph(graph)
{
	HRESULT hr = NOERROR;
	m_output = new COutputPin(this, &m_cs, &hr);
}

COutputGraph::COutputFilter::~COutputFilter()
{
	delete m_output;
}

int COutputGraph::COutputFilter::GetPinCount()
{
	return 1;
}

CBasePin *COutputGraph::COutputFilter::GetPin(int n)
{
	return m_output;
}

COutputGraph::COutputFilter::COutputPin::COutputPin(COutputFilter *pFilter, CCritSec *cs, HRESULT *phr)
: CBaseOutputPin(NAME("Output"), pFilter, cs, phr, L"Output"), m_queue(NULL)
{
}

HRESULT COutputGraph::COutputFilter::COutputPin::GetMediaType(int iPosition, CMediaType *pmt)
{
	if (iPosition > 0) return VFW_S_NO_MORE_ITEMS;
	*pmt = (((COutputFilter*)m_pFilter)->m_graph->m_type);
	return S_OK;
}

HRESULT COutputGraph::COutputFilter::COutputPin::CheckMediaType(const CMediaType *pmt)
{
	if (*pmt != (((COutputFilter*)m_pFilter)->m_graph->m_type)) return S_FALSE;
	return S_OK;
}


HRESULT COutputGraph::COutputFilter::COutputPin::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *ppropInputRequest)
{
	ALLOCATOR_PROPERTIES actual;
	CMediaType mt = ((COutputFilter*)m_pFilter)->m_graph->m_type;
	if (ppropInputRequest->cbAlign < 1) ppropInputRequest->cbAlign = 1;
	if ((unsigned)ppropInputRequest->cbBuffer < (unsigned)mt.lSampleSize) ppropInputRequest->cbBuffer = mt.lSampleSize;
	int queue = ((COutputFilter*)m_pFilter)->m_graph->m_queue;
	queue = queue > 1 ? queue : 1;
	if (ppropInputRequest->cBuffers<queue) ppropInputRequest->cBuffers = queue;
	pAlloc->SetProperties(ppropInputRequest, &actual);
	return S_OK;
}

HRESULT COutputGraph::COutputFilter::COutputPin::Deliver(IMediaSample *pSample)
{
	if (m_queue) {
		pSample->AddRef();
		return m_queue->Receive(pSample);
	}
	else
		return CBaseOutputPin::Deliver(pSample);
}

HRESULT COutputGraph::COutputFilter::COutputPin::DeliverEndOfStream()
{
	if (m_queue) {
		m_queue->EOS();
		return S_OK;
	}
	else
		return CBaseOutputPin::DeliverEndOfStream();
}

HRESULT COutputGraph::COutputFilter::COutputPin::Active()
{
	int queue = ((COutputFilter*)m_pFilter)->m_graph->m_queue;
	if (queue > 0) {
		HRESULT hr = S_OK;
		m_queue = new COutputQueue(GetConnected(), &hr, FALSE, TRUE, 1, FALSE, queue, THREAD_PRIORITY_ABOVE_NORMAL);
	}
	return CBaseOutputPin::Active();
}

HRESULT COutputGraph::COutputFilter::COutputPin::Inactive()
{
	if (m_queue) {
		delete m_queue;
		m_queue = NULL;
	}
	return CBaseOutputPin::Inactive();
}

/////////////////////////////////////////////////////////////////////////////

CAVIReader::CAVIReader(LPCSTR filename)
{
	HRESULT hr;

	IBaseFilter *pFSRC;
	WCHAR wbuf[256];
	MultiByteToWideChar(CP_ACP, 0, filename, -1, wbuf, sizeof wbuf/ sizeof (WCHAR));
	hr = m_FG->AddSourceFilter(wbuf, L"File source", &pFSRC);
	IBaseFilter * pAVI;
	hr = CoCreateInstance((REFCLSID)CLSID_AviSplitter,
						  NULL, CLSCTX_INPROC, (REFIID)IID_IBaseFilter,
						  (void **)&pAVI);
	hr = m_FG->AddFilter(pAVI, L"AVI Splitter");
	CHECK_HR(hr);
	hr = m_GB->RenderStream(NULL, NULL, pFSRC, NULL, pAVI);
	pFSRC->Release();
	CHECK_HR(hr);
	hr = m_GB->RenderStream(NULL, &MEDIATYPE_Interleaved, pAVI, NULL, m_inputFilter);
	if (hr != NOERROR || !m_inputFilter->m_input->IsConnected()) {
 	  IBaseFilter * pDVMux;
	  hr = CoCreateInstance((REFCLSID)CLSID_DVMux,
						  NULL, CLSCTX_INPROC, (REFIID)IID_IBaseFilter,
						  (void **)&pDVMux);
	  hr = m_FG->AddFilter(pDVMux, L"DV muxer");
  	  CHECK_HR(hr);
	  hr = m_GB->RenderStream(NULL, &MEDIATYPE_Video, pAVI, NULL, pDVMux);
	  hr = m_GB->RenderStream(NULL, &MEDIATYPE_Audio, pAVI, NULL, pDVMux);
	  hr = m_GB->RenderStream(NULL, NULL, pDVMux, NULL, m_inputFilter);
	  pDVMux->Release();
	}
	pAVI->Release();
#ifdef DEBUG
	DumpGraph(m_FG, 0);
#endif
}

/////////////////////////////////////////////////////////////////////////////

static UINT JoinerThread(LPVOID ptr)
{
	((CAVIJoiner *)ptr)->JoinerThread();
	return 0;
}

static int CompareStrings(const void *a, const void *b)
{
	return ((CString *)a)->CompareNoCase(*(CString *)b);
}

CAVIJoiner::CAVIJoiner(LPCSTR filenames)
: m_joinHandler(NULL), m_current(0), m_stopping(FALSE),
  m_reader(NULL), m_thread(NULL)
{
	CString flnms = filenames;
	
	while (!flnms.IsEmpty()) {

		CString filename = flnms.SpanExcluding("|");
		flnms = filename.GetLength() < flnms.GetLength() ? flnms.Mid(filename.GetLength()+1) : "";

		filename.TrimLeft(); filename.TrimRight();

		if (!filename.IsEmpty()) {
			CArray<CString,CString&> files;
			CFileFind finder;
			BOOL found;
			found = finder.FindFile(filename);
			if (!found) ThrowDShowException(CDShowException::error, filename + ": file not found");
			while (found) {
				found = finder.FindNextFile();
				files.Add(finder.GetFilePath());
			}
			if (files.GetSize()) {
				CString *ptr = files.GetData();
				qsort(ptr, files.GetSize(), sizeof (CString), CompareStrings);
			}
			for(int i = 0; i<files.GetSize(); i++) {
				m_filenames.Add(files[i]);
			}

		}

	}
	if (m_current < m_filenames.GetSize()) {
		m_reader = new CAVIReader(m_filenames[m_current]);
		m_current++;
	}
	else {
		ThrowDShowException(CDShowException::error, "No file selected");
	}

}

CAVIJoiner::~CAVIJoiner()
{
	Stop();
	delete m_reader;
}

void CAVIJoiner::GetMediaType(CMediaType *type)
{
	m_reader->GetMediaType(type);
}

void CAVIJoiner::Run(CFrameHandler *handler)
{
	m_joinHandler = handler;

	m_thread = AfxBeginThread(::JoinerThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_thread->m_bAutoDelete = FALSE;
	m_thread->ResumeThread();

	m_reader->Run(this);
}

void CAVIJoiner::Stop()
{
	m_stopping = TRUE;
	if (m_thread) {
		m_ev.SetEvent();
		WaitForSingleObject(m_thread->m_hThread, INFINITE);
		delete m_thread;
		m_thread = NULL;
	}
	delete m_reader;
	m_reader = NULL;
	m_stopping = FALSE;
	m_joinHandler = NULL;
}

void CAVIJoiner::HandleFrame(REFERENCE_TIME duration, BYTE *data, int len)
{
	if (m_stopping) 
		return;

	if (data) {
		m_joinHandler->HandleFrame(duration, data, len);
	}
	else {
		m_ev.SetEvent();
	}
}

void CAVIJoiner::JoinerThread()
{
	for(;;) {
		CSingleLock lck(&m_ev);
		lck.Lock();

		if (m_stopping) return;

		if (m_current < m_filenames.GetSize()) {
			delete m_reader;
			m_reader = new CAVIReader(m_filenames[m_current]);
			m_current++;
			m_reader->Run(this);
		}
		else {
			m_joinHandler->HandleFrame(-1, NULL, 0);
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

CDVControl::CDVControl()
: m_ET(NULL)
{
}

CDVControl::~CDVControl()
{
	if (m_ET) m_ET->Release();
}

void CDVControl::CtrlAttach(IUnknown *pDev)
{
	HRESULT hr;
	if (m_ET) m_ET->Release();
	m_ET = NULL;
	hr = pDev->QueryInterface(IID_IAMExtTransport, (void**)&m_ET);
}

void CDVControl::CtrlStop()
{
	if (m_ET) {
		m_ET->put_Mode(ED_MODE_STOP);
	}
}

void CDVControl::CtrlPlay()
{
	if (m_ET) {
		m_ET->put_Mode(ED_MODE_PLAY);
	}
}

void CDVControl::CtrlPause()
{
	if (m_ET) {
		m_ET->put_Mode(ED_MODE_PLAY);
		m_ET->put_Mode(ED_MODE_FREEZE);
	}
}

void CDVControl::CtrlRecord()
{
	if (m_ET) {
		m_ET->put_Mode(ED_MODE_RECORD);
	}
}

void CDVControl::CtrlRecPause()
{
	if (m_ET) {
		m_ET->put_Mode(ED_MODE_RECORD_FREEZE);
	}
}

/////////////////////////////////////////////////////////////////////////////

CDVInput::CDVInput(LPCSTR vsrc)
: m_DF(NULL)
{
	HRESULT hr;
	IBaseFilter *pVSRC;
	CArray<CString,CString&> list;
	EnumVideoDevices(FALSE, vsrc, list, &pVSRC);
	CtrlAttach(pVSRC);
	hr = m_FG->AddFilter(pVSRC, L"DVin");
	CHECK_HR(hr);

	hr = m_GB->RenderStream(NULL, &MEDIATYPE_Interleaved, pVSRC, NULL, m_inputFilter);
	pVSRC->Release();
	CHECK_HR(hr);


	IPin *pPin;
	hr = m_inputFilter->m_input->ConnectedTo(&pPin);
	CHECK_HR(hr, "Can't find DV output pin");
	hr = pPin->QueryInterface(IID_IAMDroppedFrames, (void**)&m_DF);
	pPin->Release();
	CHECK_HR(hr, "Can't find IAMDroppedFrames");

#ifdef DEBUG
	DumpGraph(m_FG, 0);
#endif
}

CDVInput::~CDVInput()
{
	if (m_DF) m_DF->Release();
}

long CDVInput::GetDroppedFrames()
{
	long dropped = -1;
	m_DF->GetNumDropped(&dropped);
	return dropped;
}

/////////////////////////////////////////////////////////////////////////////

CDVOutput::CDVOutput(LPCSTR vdst, CMediaType *type)
: COutputGraph(type, 10)
{
	HRESULT hr;
	IBaseFilter *pVDST;
	CArray<CString,CString&> list;
	EnumVideoDevices(TRUE, vdst, list, &pVDST);
	CtrlAttach(pVDST);
	hr = m_FG->AddFilter(pVDST, L"DVout");
	CHECK_HR(hr);
	hr = m_GB->RenderStream(NULL, NULL, (IBaseFilter*)m_outputFilter, NULL, pVDST);
	pVDST->Release();
	CHECK_HR(hr);
#ifdef DEBUG
	DumpGraph(m_FG, 0);
#endif
	hr = m_MC->Run();
	if (hr != S_OK) {
		OAFilterState state;
		hr = m_MC->GetState(1000, &state);
		CHECK_HR(hr, "Can't start DV output");
		if (state != State_Running)
			ThrowDShowException(CDShowException::error, "DV output not running");
	}
}

CDVOutput::~CDVOutput()
{
	m_outputFilter->m_output->DeliverEndOfStream();
	if (m_ME) {
		long evCode;
		m_ME->WaitForCompletion(5000, &evCode);
	}
}

/////////////////////////////////////////////////////////////////////////////

CAVIWriter::CAVIWriter(LPCSTR filename, LPCSTR dtformat, int ndigits, time_t tim, bool type2AVI, CMediaType *type)
: COutputGraph(type), m_filename(filename), m_dtformat(dtformat), m_ndigits(ndigits), m_dvtime(tim)
{
	HRESULT hr;
	IBaseFilter * pMux; IFileSinkFilter *pFile;
	WCHAR wbuf[256];
	CString tmp = "~";
	m_tmpfile = GetCaptureFilename(m_filename, tmp+m_dtformat, m_ndigits, m_dvtime);
	MultiByteToWideChar(CP_ACP, 0, m_tmpfile, -1, wbuf, sizeof wbuf/ sizeof (WCHAR));
	m_GB->SetOutputFileName(&MEDIASUBTYPE_Avi, wbuf, &pMux, &pFile);
	IFileSinkFilter2 *pFile2;
	hr = pFile->QueryInterface(IID_IFileSinkFilter2, (void**)&pFile2);
	pFile->Release();
	pFile2->SetMode(AM_FILE_OVERWRITE);
	pFile2->Release();
	if (type2AVI) {
		IBaseFilter *pDVSplit;
		hr = CoCreateInstance((REFCLSID)CLSID_DVSplitter,
							  NULL, CLSCTX_INPROC, (REFIID)IID_IBaseFilter,
							  (void **)&pDVSplit);
		CHECK_HR(hr);
		hr = m_FG->AddFilter(pDVSplit, L"DV splitter");
		CHECK_HR(hr);
		hr = m_GB->RenderStream(NULL, &MEDIATYPE_Interleaved, (IBaseFilter*)m_outputFilter, NULL, pDVSplit);
		CHECK_HR(hr);
		hr = m_GB->RenderStream(NULL, &MEDIATYPE_Video, pDVSplit, NULL, pMux);
		CHECK_HR(hr);
		hr = m_GB->RenderStream(NULL, &MEDIATYPE_Audio, pDVSplit, NULL, pMux);
		CHECK_HR(hr);
		pDVSplit->Release();
	}
	else {
		hr = m_GB->RenderStream(NULL, &MEDIATYPE_Interleaved, (IBaseFilter*)m_outputFilter, NULL, pMux);
		CHECK_HR(hr);
	}
	pMux->Release();
#ifdef DEBUG
	DumpGraph(m_FG, 0);
#endif
	m_MC->Run();
}

CAVIWriter::~CAVIWriter()
{
	m_outputFilter->m_output->DeliverEndOfStream();
	if (m_ME) {
		long evCode;
		m_ME->WaitForCompletion(5000, &evCode);
	}
	if (m_MC) m_MC->Stop();
	CString tmp = GetCaptureFilename(m_filename, m_dtformat, m_ndigits, m_dvtime);
	MoveFile(m_tmpfile, tmp);
}

/////////////////////////////////////////////////////////////////////////////

UINT MonitoringThread(LPVOID ptr)
{
	((CMonitor*)ptr)->MonitoringThread();
	return 0;
}

CMonitor::CMonitor(HWND hWnd, CMediaType *type)
: COutputGraph(type), m_hWnd(hWnd), m_VW(NULL), m_sample(NULL)
{
	HRESULT hr;
	hr = m_FG->QueryInterface(IID_IVideoWindow, (void **)&m_VW);
	CHECK_HR(hr);
	hr = m_GB->RenderStream(NULL, NULL, (IBaseFilter*)m_outputFilter, NULL, NULL);
	SetDVDecoding(m_FG, 0);
	hr = m_VW->put_Owner((LONG)m_hWnd);
	hr = m_VW->put_WindowStyle(WS_CHILD);
	Resize();
	m_MC->Run();

	m_thread = AfxBeginThread(::MonitoringThread,this,THREAD_PRIORITY_BELOW_NORMAL,0,CREATE_SUSPENDED);
	m_thread->m_bAutoDelete = FALSE;
	m_thread->ResumeThread();
}

CMonitor::~CMonitor()
{
	if (m_VW) m_VW->Release();
	m_VW = NULL;
	m_ev.SetEvent();

	WaitForSingleObject(m_thread->m_hThread, INFINITE);
	delete m_thread;
}

void CMonitor::Resize() {
	ULONG cx, cy, w, h;
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	cx = rect.right - rect.left;
	cy = rect.bottom - rect.top;

	w = cy*4/3;//*m_width/m_height;
	h = cx*3/4;//*m_height/m_width;
	if (cx < w) w = cx;
	if (cy < h) h = cy;
	m_VW->SetWindowPosition((cx-w)/2, (cy-h)/2, w, h);
}

void CMonitor::HandleFrame(REFERENCE_TIME duration, BYTE *data, int len)
{
	if (data) {
		if (m_sample) {
			BYTE *ptr;
			m_sample->GetPointer(&ptr);
			CopyMemory(ptr, data, len);
			m_sample->SetActualDataLength(len);
			m_sample->SetSyncPoint(true);
			m_sample = NULL;
			m_ev.SetEvent();
		}
	}
}

void CMonitor::MonitoringThread()
{
	DWORD ticks = 0;
	for(;;) {
		IMediaSample *pSample = NULL;
		HRESULT hr;
		hr = m_outputFilter->m_output->GetDeliveryBuffer(&pSample, NULL, NULL, 0);
		if (hr == NOERROR) {
			ticks = GetTickCount() - ticks;
			Sleep(ticks < 200 ? ticks + 10 : 200);
			m_sample = pSample;
			CSingleLock lck(&m_ev);
			lck.Lock();
			if (!m_VW) {
				pSample->Release();
				return;
			}
			ticks = GetTickCount();
			m_outputFilter->m_output->Deliver(pSample);
			pSample->Release();
			pSample = NULL;
		}
		else {
			Sleep(100);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

CDVQueue::CDVQueue(int queueSize, int dataSize)
: m_buffers(NULL), m_queue(NULL), m_dataSize(dataSize), m_queueSize(queueSize+1),
  m_head(0), m_tail(0), m_load(0), m_end(false)
{
	m_buffers = new BYTE[(sizeof (Buffer) + m_dataSize) * m_queueSize];
	m_queue = new Buffer *[m_queueSize];
	for(int i=0; i<m_queueSize; i++) {
		m_queue[i] = (Buffer *) (m_buffers + (sizeof (Buffer) + m_dataSize) * i);
	}
}

CDVQueue::~CDVQueue()
{
	delete m_buffers;
	delete[] m_queue;
}

void CDVQueue::Put(REFERENCE_TIME duration, BYTE *data, int len)
{
	if (!data) {
		m_end = true;
		m_evGet.SetEvent();
		m_evPut.SetEvent();
		return;
	}
	while(!m_end) {
		{
			CAutoLock lock(&m_cs);
			if (m_load < m_queueSize-1) {
				m_queue[m_tail]->duration = duration;
				m_queue[m_tail]->len = len;
				CopyMemory(m_queue[m_tail]->data, data, len);
				m_tail = (m_tail+1) % m_queueSize;
				m_load++;
				m_evGet.SetEvent();
				break;
			}
		}
		CSingleLock lck(&m_evPut);
		lck.Lock();
	}
}


bool CDVQueue::Get(REFERENCE_TIME *duration, BYTE **data, int *len)
{
	for(;;) {
		{
			CAutoLock lock(&m_cs);
			if (m_load > 0) {
				*duration = m_queue[m_head]->duration;
				*len = m_queue[m_head]->len;
				*data = m_queue[m_head]->data;
				m_head = (m_head+1) % m_queueSize;
				m_load--;
				m_evPut.SetEvent();
				break;
			}
		}
		if (m_end) return false;
		CSingleLock lck(&m_evGet);
		lck.Lock();
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////

static UINT RecordingThread(LPVOID pdv)
{
	((CDV *)pdv)->RecordingThread();
	return 0;
}
static UINT CapturingThread(LPVOID pdv)
{
	((CDV *)pdv)->CapturingThread();
	return 0;
}

CDV::CDV()
: m_aviJoiner(NULL), m_aviWriter(NULL), m_dvInput(NULL), m_dvOutput(NULL), m_monitor(NULL), m_queue(NULL),
  m_thread(NULL),
  m_type2AVI(true), m_discontinuityTreshold(1), m_maxAVIFrames(25*60*15), m_everyNth(1), m_recordPreview(TRUE),
  m_dropped(0), m_counter(-1), m_time(-1), m_captureTime(0), m_ndigits(0), m_DVctrl(FALSE)
{
}

CDV::~CDV()
{
	Destroy();
}

void CDV::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);

	if (m_monitor) m_monitor->Resize();	
}

int CDV::GetState()
{
	return m_state;
}

int CDV::GetDropped()
{
	return m_dropped;
}

long CDV::GetCounter()
{
	return m_counter;
}

REFERENCE_TIME CDV::GetTime()
{
	return m_time;
}

int CDV::GetQueueLoad()
{
	return m_queue->m_load;
}


CString CDV::GetCaptureFilename()
{
	CAutoLock lock(&m_cs);
	
	return m_captureFilename;
}

void CDV::Destroy()
{
	m_state = Idle;

	if (m_queue) {m_queue->Put(-1, NULL, 0);}

	if (m_aviJoiner) m_aviJoiner->Stop();
	if (m_dvInput) m_dvInput->Stop();

	if (m_thread) {
		WaitForSingleObject(m_thread->m_hThread, INFINITE);
		delete m_thread;
		m_thread = NULL;
	}

	if (m_aviJoiner) { delete(m_aviJoiner);	m_aviJoiner = NULL; }
	if (m_dvInput) { 
		if (m_DVctrl) m_dvInput->CtrlStop();
		delete(m_dvInput);	m_dvInput = NULL; 
	}
	if (m_aviWriter) { delete(m_aviWriter);	m_aviWriter = NULL; }
	if (m_dvOutput) { 
		if (m_DVctrl) m_dvOutput->CtrlStop();
		delete(m_dvOutput); m_dvOutput = NULL; 
	}
	if (m_monitor) { delete(m_monitor);	m_monitor = NULL; }
	if (m_queue) { delete(m_queue);	m_queue = NULL; }
	m_dropped = 0;
	m_counter = -1;
	m_time = -1;
	m_captureTime = 0;
}


void CDV::HandleFrame(REFERENCE_TIME duration, BYTE *data, int len)
{
	m_queue->Put(duration, data, len);
}


void CDV::BuildCapturing(LPCSTR vsrc)
{
	Destroy();
	HRESULT hr = S_OK;

	m_dvInput = new CDVInput(vsrc);

	CMediaType type;
	m_dvInput->GetMediaType(&type);

	m_monitor = new CMonitor(m_hWnd, &type);

	m_queue = new CDVQueue(100, type.GetSampleSize());

	m_dvInput->Run(this);

	m_state = CapturePaused;

	m_thread = AfxBeginThread(::CapturingThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_thread->m_bAutoDelete = FALSE;
	m_thread->ResumeThread();

	InvalidateRect(NULL);
	UpdateWindow();
}

void CDV::StopCapturing()
{
	if (m_state == Capturing) {
		m_state = CapturePaused;
		if (m_DVctrl) m_dvInput->CtrlPause();
	}
}

void CDV::StartCapturing(LPCSTR filename, LPCSTR dtformat, int ndigits, REFERENCE_TIME captureTime)
{
	if (m_state == CapturePaused) {
		m_captureTime = captureTime;
		m_captureFilename = filename;
		m_dtformat = dtformat;
		m_ndigits = ndigits;
		m_state = Capturing;
		if (m_DVctrl) m_dvInput->CtrlPlay();
	}
}


void CDV::BuildRecording(LPCSTR filenames, LPCSTR vdst)
{
	Destroy();

	m_aviJoiner = new CAVIJoiner(filenames);

	CMediaType type;
	m_aviJoiner->GetMediaType(&type);

	m_monitor = new CMonitor(m_hWnd, &type);
	m_dvOutput = new CDVOutput(vdst, &type);

	m_queue = new CDVQueue(100, type.GetSampleSize());

	m_aviJoiner->Run(this);

	m_state = RecordPaused;

	if (m_DVctrl) m_dvOutput->CtrlRecPause();

	m_thread = AfxBeginThread(::RecordingThread,this,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	m_thread->m_bAutoDelete = FALSE;
	m_thread->ResumeThread();

	InvalidateRect(NULL);
	UpdateWindow();
}

void CDV::StopRecording()
{
	if (m_state == Recording) {
		m_state = RecordPaused;
		if (m_DVctrl) m_dvOutput->CtrlRecPause();
	}
}

void CDV::StartRecording()
{
	if (m_state == RecordPaused) {
		m_state = Recording;
		if (m_DVctrl) m_dvOutput->CtrlRecord();
	}
}

void CDV::CapturingThread()
{
	BYTE *buffer;
	REFERENCE_TIME duration;
	int len;
	CMediaType type;
	m_dvInput->GetMediaType(&type);
	long nFrames, counter;
	long dropped = m_dvInput->GetDroppedFrames();
	int dvTime = 0, oldDVTime = 0;
	m_counter = 0;
	m_time = 0;
	while (m_state) {
		if (m_queue->Get(&duration, &buffer, &len)) {
			long newDVTime = GetDVRecordingTime(buffer, len);
			int deltaDVTime = 0;
			if (dvTime != newDVTime) {
				if (newDVTime > 0) {
					if (oldDVTime > 0) {
						deltaDVTime = newDVTime - oldDVTime;
						if (deltaDVTime < 0) deltaDVTime = -deltaDVTime;
					}
					oldDVTime = newDVTime;
				}
				dvTime = newDVTime;
				GetParent()->PostMessage(WM_DV_TIMECHANGE, 0, dvTime);
			}
			if (m_queue->m_load < m_queue->m_queueSize/2) 
				m_monitor->HandleFrame(duration, buffer, len);

			if (m_state == Capturing) {
				if (m_aviWriter && (nFrames >= m_maxAVIFrames || ((m_discontinuityTreshold > 0) && (deltaDVTime > m_discontinuityTreshold)))) {
					delete m_aviWriter;
					m_aviWriter = NULL;
				}
				if (!m_aviWriter) {
					m_aviWriter = new CAVIWriter(m_captureFilename, m_dtformat, m_ndigits, dvTime, m_type2AVI, &type);
					nFrames = 0;
					counter = 0;
				}
				if ((counter % m_everyNth) == 0) {
					m_aviWriter->HandleFrame(duration, buffer, len);
					nFrames++;
				}
				if (dvTime && !m_aviWriter->m_dvtime) {
					m_aviWriter->m_dvtime = dvTime;
				}
				counter++;
				m_counter++;
				m_time += duration;
				if (m_captureTime && (m_time >= m_captureTime)) {
					m_captureTime = 0;
					m_state = Finished;
				}
				m_dropped = m_dvInput->GetDroppedFrames() - dropped;
			}
			else {
				if (m_aviWriter) {
					delete m_aviWriter;
					m_aviWriter = NULL;
				}
				dropped = m_dvInput->GetDroppedFrames();
				if (m_state != Finished) {
					m_dropped = 0;
					m_counter = 0;
					m_time = 0;
				}
			}
		}
	}
	if (m_aviWriter) {
		delete m_aviWriter;
		m_aviWriter = NULL;
	}
	GetParent()->PostMessage(WM_DV_TIMECHANGE, 0, 0);
}

void CDV::RecordingThread()
{
	BYTE *buffer;
	REFERENCE_TIME duration;
	int len;
	long dvTime = 0;
	GetParent()->PostMessage(WM_DV_TIMECHANGE, 0, 0);
	if (m_queue->Get(&duration, &buffer, &len)) {
		m_counter = 0;
		m_time  = 0;
		while (m_state) {
			long newDVTime = GetDVRecordingTime(buffer, len);
			if (dvTime != newDVTime) {
				dvTime = newDVTime;
				GetParent()->PostMessage(WM_DV_TIMECHANGE, 0, dvTime);
			}
			if (m_recordPreview) {
				if (m_queue->m_end || m_queue->m_load > m_queue->m_queueSize/2) 
					m_monitor->HandleFrame(duration, buffer, len);
			}

			m_dvOutput->HandleFrame(duration, buffer, len);
			if (m_state == Recording) {
				m_counter++;
				m_time += duration;
				if (!m_queue->Get(&duration, &buffer, &len)) {
					if (m_state) m_state = Finished;
				}
			}
		}
	}
	GetParent()->PostMessage(WM_DV_TIMECHANGE, 0, 0);
}

BEGIN_MESSAGE_MAP(CDV, CStatic)
	//{{AFX_MSG_MAP(CDV)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

CString FormatTime(LPCSTR format, time_t tim)
{
	CString tmp = format;
	char buf[1024];

	strftime(buf, sizeof buf, tmp + '\0', localtime(&tim));

	return CString(buf);
}

