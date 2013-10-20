#include <kruto/kruto.hpp>

using namespace kruto;

class BasicApp : public Application
{
public:
	BasicApp(const int pArgc, char **pArgv) : 
		Application(pArgc, pArgv, "Demos: Basic", 800, 600), 
		mSoldierX(100 + Math::random(100)), 
		mSoldierY(120 + Math::random(120))
	{
		// empty
	}

	virtual bool initialize(void)
	{
		if(!Application::initialize())
			return false;

		if(!mBackground.load("data/gfx/greenground1.png"))
			return false;

		if(!mBuilding.load("data/gfx/building11.png"))
			return false;

		if(!mTree.load("data/gfx/fullpufftree.png"))
			return false;

		if(!mSoldier.load("data/gfx/malesoldiernormal.png"))
			return false;

#ifndef KR_NO_AUDIO
		if(!mMusic.load("data/sfx/calm_town.ogg"))
			return false;

		mMusic.setLoop(true);
		mMusic.play();
#endif

		mTrees.setImage(&mTree);
		mTrees.reserve(2);

		mTrees.set(0, 400, 100, 144, 196);
		mTrees.set(1, 300, 300, 144, 196);

		return true;
	}

	virtual void uninitialize(void)
	{
#ifndef KR_NO_AUDIO
		mMusic.dispose();
#endif

		mSoldier.dispose();
		mTree.dispose();
		mBuilding.dispose();
		mBackground.dispose();

		Application::uninitialize();
	}

	virtual void onKeyDown(const KeyEvent *pKeyEvent)
	{
		Application::onKeyDown(pKeyEvent);

		switch(pKeyEvent->key())
		{
			case Keys::Escape:	
				quit();
				break;
		}
	}

	virtual void update(const unsigned int pDelta)
	{
		if(isKeyDown(Keys::Left))
			mSoldierX -= pDelta * 0.10;
		else if(isKeyDown(Keys::Right))
			mSoldierX += pDelta * 0.10;

		if(isKeyDown(Keys::Up))
			mSoldierY -= pDelta * 0.10;
		else if(isKeyDown(Keys::Down))
			mSoldierY += pDelta * 0.10;

		if(isKeyDown(Keys::Space))
			mTrees.translate(1, pDelta * 0.10, 0);
	}

	virtual void draw(void)
	{
		Canvas::begin();
		Canvas::drawImage(&mBackground, 0, 0);
		Canvas::drawImage(&mBuilding, 20, 20);
		Canvas::drawImage(&mSoldier, 0, 128, 64, 64, mSoldierX, mSoldierY, 64, 64);
		Canvas::drawImageBatch(&mTrees);
		Canvas::end();
	}

private:
	Image mBackground;
	Image mBuilding;
	Image mTree;
	Image mSoldier;

	ImageBatch mTrees;

	Audio mMusic;

	float mSoldierX;
	float mSoldierY;
};

KR_MAIN(BasicApp)
