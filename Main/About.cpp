/*
 * Copyrights (c):
 *     2000 - 2008 , Werner Freytag.
 *     2009, Haiku
 * Distributed under the terms of the MIT License.
 *
 * Original Author:
 *              Werner Freytag <freytag@gmx.de>
 */

#include <stdlib.h>

#include <Application.h>
#include <Window.h>
#include <Resources.h>
#include <Bitmap.h>
#include <Message.h>
#include <View.h>
#include <Button.h>
#include <Entry.h>
#include <Node.h>
#include <NodeInfo.h>
#include <Roster.h>
#include <Font.h>
#include <Mime.h>
#include <String.h>
#include <Path.h>

#include "About.h"

const int32	MSG_ABOUT_OK = 'AbOK';

class	AboutView : public BView {

public:
					AboutView(BRect frame, const char *ProgrammName, const char *Release, const char **BodyText, 
						const char* ButtonText,	const char *Site, const char *EMail);
	virtual	void	Draw(BRect updateRect);
	virtual	void	MessageReceived(BMessage *msg);
	virtual	void	AttachedToWindow();
	virtual	void	MouseDown(BPoint where);
	virtual	void	MouseUp(BPoint where);
	virtual	void	MouseMoved(	BPoint where, uint32 code, const BMessage *a_message);

private:
	void			InitPrint(bool aktiv);
	void			PrintMail(bool aktiv);
	void			PrintSite(bool aktiv);
	
	BBitmap			*fLogo;
	BBitmap			*fMuster;
	BBitmap			*fIcon;
	BBitmap			*fMailIcon, *fSiteIcon;
	BString			fMailCall, fSiteCall;
	BRect			fMailRect, fSiteRect;
	float			fBaseLine;
	bool			fMouseDown, fMailPressed, fSitePressed;
	const char		*fProgrammName, *fRelease, *fButtonText, *fSite, *fEMail;
	BString			**fBodyText;
	char			fNumberOfBodyTextLines;
};

class AboutWindow : public BWindow {
	public:
		AboutWindow(BRect frame, const char *FensterTitel, const char *ProgrammName, const char *Release, const char **BodyText, const char *ButtonText, const char *Site, const char *EMail)
			: BWindow( frame, FensterTitel, B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_RESIZABLE|B_NOT_ZOOMABLE) {
			AboutView	*myView = new AboutView(Bounds(), ProgrammName, Release, BodyText, ButtonText, Site, EMail);
			AddChild(myView);
			Show();
		}
		virtual bool	QuitRequested() { Hide(); return false; }
};

AboutView::AboutView(BRect frame, const char *ProgrammName, const char *Release, const char **BodyText, 
	const char* ButtonText,	const char *Site, const char *EMail) : BView(frame, NULL, B_FOLLOW_ALL, B_WILL_DRAW) {

	fProgrammName 	= ProgrammName;
	fRelease		= Release;
	fButtonText		= ButtonText;
	fSite			= Site;
	fEMail			= EMail;

	for (fNumberOfBodyTextLines=0; BodyText[fNumberOfBodyTextLines] != NULL; fNumberOfBodyTextLines++);
	
	fBodyText = new BString *[fNumberOfBodyTextLines];
	for (int i=0; i<fNumberOfBodyTextLines; i++) fBodyText[i] = new BString(BodyText[i]);

	BResources		*Resourcen = be_app->AppResources();

	size_t			Groesse;
	BMessage		*myMessage = new BMessage();
	char			*buf = (char *)Resourcen->LoadResource('BBMP', "about:hg", &Groesse);

	myMessage->Unflatten(buf);
	fMuster = new BBitmap(myMessage);
	delete (myMessage);

	buf = (char *)Resourcen->LoadResource('BBMP', "about:logo", &Groesse);
	myMessage->Unflatten(buf);
	fLogo = new BBitmap(myMessage);

	app_info	myAppInfo;
	be_app->GetAppInfo(&myAppInfo);
	
	BEntry		myEntry = BEntry(&myAppInfo.ref, true);
	BNode 		myNode = BNode(&myEntry);
	BNodeInfo	myNodeInfo(&myNode);
	
	fIcon		= new BBitmap(BRect(0,0,31,31), B_RGBA32 );
	myNodeInfo.GetTrackerIcon(fIcon);

	entry_ref	ref;

	char		Signatur[B_MIME_TYPE_LENGTH];
	fMailIcon = new BBitmap( BRect(0,0,15,15), B_RGBA32 );
	fSiteIcon = new BBitmap( BRect(0,0,15,15), B_RGBA32 );

	BMimeType("text/x-email").GetPreferredApp(Signatur);
	if ( BMimeType(Signatur).GetIcon(fMailIcon, B_MINI_ICON) != B_OK )
		if ( BMimeType(Signatur).GetIconForType("text/x-email", fMailIcon, B_MINI_ICON) != B_OK )
			BMimeType("text/x-email").GetIcon(fMailIcon, B_MINI_ICON);
	BMimeType(Signatur).GetAppHint(&ref);
	fMailCall = BString(BPath(&ref).Path()).Append(" mailto:").Append(fEMail).Append(" &");

	BMimeType("text/html").GetPreferredApp(Signatur);
	if ( BMimeType(Signatur).GetIcon(fSiteIcon, B_MINI_ICON) != B_OK )
		if ( BMimeType(Signatur).GetIconForType("text/html", fSiteIcon, B_MINI_ICON) != B_OK )
			BMimeType("text/html").GetIcon(fSiteIcon, B_MINI_ICON);
	BMimeType(Signatur).GetAppHint(&ref);
	fSiteCall = BString(BPath(&ref).Path()).Append(" ").Append(fSite).Append(" &");

}

inline void AboutView::InitPrint(bool aktiv) {
	SetLowColor( 40, 70, 150 );
	if (aktiv) SetHighColor( 255, 0, 0 ); else SetHighColor( 255, 255, 255 );
	BFont	myFont(be_plain_font);
	myFont.SetSize(11);
	SetFont(&myFont);
}

inline void AboutView::PrintMail(bool aktiv = false) {
	InitPrint(aktiv); DrawString(fEMail, BPoint(fMailRect.left + 24, fMailRect.top + fBaseLine) );
};

inline void AboutView::PrintSite(bool aktiv = false) {
	InitPrint(aktiv); DrawString(fSite, BPoint(fSiteRect.left + 24, fSiteRect.top + fBaseLine) );
};

void	AboutView::Draw(BRect updateRect) {
	SetLowColor( 255, 255, 255);
	FillRect(BRect(0, 0, Bounds().right, 60), B_SOLID_LOW);

	SetHighColor( 200, 200, 200 );
	StrokeLine(BPoint(0, 00), BPoint(Bounds().right, 0));
	SetHighColor( 230, 230, 230 );
	StrokeLine(BPoint(0, 1), BPoint(Bounds().right, 1));

	DrawBitmapAsync( fLogo, BPoint(Bounds().right - fLogo->Bounds().Width() - 10, 10) );

	SetHighColor( 0, 0, 0 );
	SetLowColor( 255, 255, 255 );

	BFont	myFont(be_plain_font);

	myFont.SetFamilyAndFace("Dutch801 Rm BT", B_ITALIC_FACE);
	myFont.SetSize(30);
	SetFont(&myFont);

	DrawString(fProgrammName, BPoint(10, 30));
	
	float StringLaenge = myFont.StringWidth(fProgrammName);
	
	myFont = BFont(be_plain_font);

	myFont.SetSize(12);
	SetFont(&myFont);
	
	DrawString( fRelease, BPoint(StringLaenge - myFont.StringWidth(fRelease) + 5, 50) );

	SetDrawingMode(B_OP_OVER);
	DrawBitmapAsync( fIcon, BPoint(10, 25) );
	SetDrawingMode(B_OP_COPY);

	SetLowColor( 40, 70, 150 );
	SetHighColor( 255, 255, 255 );

	for (int i=0; i<fNumberOfBodyTextLines; i++) DrawString(fBodyText[i]->String(), BPoint(10, 80 + i * 15));

	SetDrawingMode(B_OP_OVER);
	DrawBitmap( fMailIcon, fMailRect.LeftTop() );
	SetDrawingMode(B_OP_COPY);
	PrintMail();

	SetDrawingMode(B_OP_OVER);
	DrawBitmap( fSiteIcon, fSiteRect.LeftTop() );
	SetDrawingMode(B_OP_COPY);
	PrintSite();
};

void	AboutView::MessageReceived(BMessage *msg) {
	switch (msg->what) {
		case MSG_ABOUT_OK:
		case B_QUIT_REQUESTED:
			Window()->Hide();
			break;
		default:
			BView::MessageReceived(msg);
	}
};

void AboutView::MouseDown(BPoint where) {
	do {
		if ( fMailRect.Contains(where) ) {
			fMouseDown = true; fMailPressed = true;
			PrintMail(true);
			break;
		}
		if ( fSiteRect.Contains(where) ) {
			fMouseDown = true; fSitePressed = true;
			PrintSite(true);
			break;
		}
	} while (false);
}

void AboutView::MouseUp(BPoint where) {
	fMouseDown = false;
	do {
		if ( fMailPressed && fMailRect.Contains(where)) {
			PrintMail();
			system( fMailCall.String() );
			break;
		}
		if ( fSitePressed && fSiteRect.Contains(where) ) {
			PrintSite();
			system( fSiteCall.String() );
			break;
		}
	} while (false);
	fSitePressed = false; fMailPressed = false;
};

void AboutView::MouseMoved(	BPoint where, uint32 code, const BMessage *a_message) {
	if (!fMouseDown) return;
	
	GetMouse( &where, &code);
	if (code !=	B_PRIMARY_MOUSE_BUTTON) {
		fMouseDown = false; fMailPressed = false; fSitePressed = false;
		PrintMail(); PrintSite();
		return;
	}
	
	do {
		if ( fMailPressed && fMailRect.Contains(where) ) {
			PrintMail(true);
			break;
		}
		if ( fSitePressed && fSiteRect.Contains(where) ) {
			PrintSite(true);
			break;
		}			
		PrintMail(); PrintSite();
	} while (false);
};

void AboutView::AttachedToWindow() {
	SetViewBitmap(fMuster);
	
	BRect	ButtonFrame(Bounds().Width() - be_plain_font->StringWidth(fButtonText) - 30, Bounds().bottom - be_plain_font->Size() - 24, Bounds().Width() - 12, Bounds().bottom - 20);
	if ( ButtonFrame.Width() < 80 ) ButtonFrame.left = ButtonFrame.right - 80;

	BButton		*OkButton = new BButton(ButtonFrame, NULL, fButtonText, new BMessage(MSG_ABOUT_OK));
	OkButton->MakeDefault(true);
	OkButton->SetTarget(this);
	
	AddChild( OkButton );
	OkButton->SetViewBitmap(fMuster);
	
	BFont	myFont(be_plain_font);
	myFont.SetSize(11);
	
	font_height	FontHoehen;

	myFont.GetHeight(&FontHoehen);
	fBaseLine = FontHoehen.ascent;
	
	fMailRect = BRect( 10, Bounds().bottom - 38, myFont.StringWidth(fEMail) + 34, Bounds().bottom - 38 + FontHoehen.ascent + FontHoehen.descent);
	fSiteRect = BRect( 10, Bounds().bottom - 20, myFont.StringWidth(fSite) + 34, Bounds().bottom - 20 + FontHoehen.ascent + FontHoehen.descent);
	
};

void About(const char *FensterTitel, const char *ProgrammName, const char *Release, const char **BodyText, const char* ButtonText,
	const char *Site, const char *EMail) {

	static BWindow	*fAboutWindow = NULL;
	
	// Doppelter Aufruf? Fenster vorholen!
	if (fAboutWindow != NULL) {
		fAboutWindow->Lock();
		if (fAboutWindow->IsHidden()) fAboutWindow->Show();
		else fAboutWindow->Minimize(false);
		fAboutWindow->Unlock();
		return;
	}
	
	short BodyTextCount = 0; while ( BodyText[BodyTextCount] != NULL ) BodyTextCount++;
	float halbeHoehe = BodyTextCount * 7.5 + 62; if ( halbeHoehe < 90 ) halbeHoehe = 90;
	
	BRect	frame(140, 240 - halbeHoehe, 500, 240 + halbeHoehe);
	fAboutWindow = new AboutWindow(frame, FensterTitel, ProgrammName, Release, BodyText, ButtonText, Site, EMail);
}
