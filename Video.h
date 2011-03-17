#ifndef VIDEO_H_
#define VIDEO_H_

class AVFormatContext;
class AVCodecContext;
class AVCodec;
class AVFrame;

class Video
{
public:
	Video();
	virtual ~Video();

	bool load(const char* filename);

	void nextFrame();
	void render();

	void uploadFrame(AVFrame* pFrame, int width, int height);

private:
	AVFormatContext* m_pFormatCtx;
	AVCodecContext* m_pCodecCtx;
	AVCodec* m_pCodec;
	AVFrame* m_pFrame;
	AVFrame* m_pFrameRGB;
	unsigned char* m_pBuffer;
	int m_nBufferSize;
	int m_nVideoStream;
	unsigned int m_unCurrentFrameGLIndex;

};

#endif /* VIDEO_H_ */
