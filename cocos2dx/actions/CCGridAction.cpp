/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "CCGridAction.h"
#include "CCDirector.h"
#include "effects/CCGrid.h"

namespace cocos2d
{
	// implementation of CCGridAction

	CCGridAction* CCGridAction::actionWithSize(cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		CCGridAction *pAction = new CCGridAction();
		if (pAction)
		{
			if (pAction->initWithSize(gridSize, duration))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_DELETE(pAction);
			}
		}

		return pAction;
	}

	bool CCGridAction::initWithSize(cocos2d::ccGridSize gridSize, cocos2d::ccTime duration)
	{
		if (__super::initWithDuration(duration))
		{
			m_sGridSize = gridSize;

			return true;
		}

		return false;
	}

	void CCGridAction::startWithTarget(CCNode *pTarget)
	{
		__super::startWithTarget(pTarget);

		CCGridBase *newgrid = grid();

		CCNode *t = dynamic_cast<CCNode*>(pTarget);
		CCGridBase *targetGrid = t->getGrid();

		if (targetGrid && targetGrid->getReuseGrid() > 0)
		{
			if (targetGrid->isActive() && targetGrid->getGridSize().x == m_sGridSize.x
				&& targetGrid->getGridSize().y == m_sGridSize.y && dynamic_cast<CCGridBase*>(targetGrid) != NULL)
			{
				targetGrid->reuse();
			}
			else
			{
				assert(0);
			}
		}
		else
		{
			if (targetGrid && targetGrid->isActive())
			{
				targetGrid->setActive(false);
			}

			t->setGrid(newgrid);
			t->getGrid()->setActive(true);
		}
	}

	CCGridBase* CCGridAction::grid(void)
	{
		// Abstract class needs implementation
		assert(0);

		return NULL;
	}

	CCIntervalAction* CCGridAction::reverse(void)
	{
		return CCReverseTime::actionWithAction(this);
	}

	NSObject* CCGridAction::copyWithZone(cocos2d::NSZone *pZone)
	{
		NSZone* pNewZone = NULL;
		CCGridAction* pCopy = NULL;
		if(pZone && pZone->m_pCopyObject) 
		{
			//in case of being called at sub class
			pCopy = dynamic_cast<CCGridAction*>(pZone->m_pCopyObject);
		}
		else
		{
			pCopy = new CCGridAction();
			pZone = pNewZone = new NSZone(pCopy);
		}

		__super::copyWithZone(pZone);

		pCopy->initWithSize(m_sGridSize, m_fDuration);
		
		CCX_SAFE_DELETE(pNewZone);
		return pCopy;
	}

	// implementation of Grid3DAction

	CCGridBase* CCGrid3DAction::grid(void)
	{
		return CCGrid3D::gridWithSize(m_sGridSize);
	}

	ccVertex3F CCGrid3DAction::vertex(cocos2d::ccGridSize pos)
	{
		CCGrid3D *g = (CCGrid3D*)dynamic_cast<CCNode*>(m_pTarget)->getGrid();
		return g->vertex(pos);
	}

	ccVertex3F CCGrid3DAction::originalVertex(cocos2d::ccGridSize pos)
	{
		CCGrid3D *g = (CCGrid3D*)dynamic_cast<CCNode*>(m_pTarget)->getGrid();
		return g->originalVertex(pos);
	}

	void CCGrid3DAction::setVertex(cocos2d::ccGridSize pos, cocos2d::ccVertex3F vertex)
	{
		CCGrid3D *g = (CCGrid3D*)dynamic_cast<CCNode*>(m_pTarget)->getGrid();
		return g->setVertex(pos, vertex);
	}

	// implementation of TiledGrid3DAction

	CCGridBase* CCTiledGrid3DAction::grid(void)
	{
		return CCTiledGrid3D::gridWithSize(m_sGridSize);
	}

	ccQuad3 CCTiledGrid3DAction::tile(cocos2d::ccGridSize pos)
	{
		CCTiledGrid3D *g = (CCTiledGrid3D*)dynamic_cast<CCNode*>(m_pTarget)->getGrid();
		return g->tile(pos);
	}

	ccQuad3 CCTiledGrid3DAction::originalTile(cocos2d::ccGridSize pos)
	{
		CCTiledGrid3D *g = (CCTiledGrid3D*)dynamic_cast<CCNode*>(m_pTarget)->getGrid();
		return g->originalTile(pos);
	}

	void CCTiledGrid3DAction::setTile(cocos2d::ccGridSize pos, cocos2d::ccQuad3 coords)
	{
		CCTiledGrid3D *g = (CCTiledGrid3D*)dynamic_cast<CCNode*>(m_pTarget)->getGrid();
		return g->setTile(pos, coords);
	}

	// implementation CCAccelDeccelAmplitude

	CCAccelDeccelAmplitude* CCAccelDeccelAmplitude::actionWithAction(cocos2d::CCAction *pAction, cocos2d::ccTime duration)
	{
		CCAccelDeccelAmplitude *pRet = new CCAccelDeccelAmplitude();
		if (pRet)
		{
			if (pRet->initWithAction(pAction, duration))
			{
				pRet->autorelease();
			}
			else
			{
				CCX_SAFE_DELETE(pRet);
			}
		}

		return pRet;
	}

	bool CCAccelDeccelAmplitude::initWithAction(cocos2d::CCAction *pAction, cocos2d::ccTime duration)
	{
		if (__super::initWithDuration(duration))
		{
			m_fRate = 1.0f;
			m_pOther = dynamic_cast<CCIntervalAction*>(pAction);
			pAction->retain();

			return true;
		}

		return false;
	}

	CCAccelDeccelAmplitude::~CCAccelDeccelAmplitude(void)
	{
		CCX_SAFE_RELEASE(m_pOther);
	}

	void CCAccelDeccelAmplitude::startWithTarget(CCNode *pTarget)
	{
		__super::startWithTarget(pTarget);
		m_pOther->startWithTarget(pTarget);
	}

	void CCAccelDeccelAmplitude::update(cocos2d::ccTime time)
	{
		float f = time * 2;

		if (f > 1)
		{
			f -= 1;
			f = 1 - f;
		}

		dynamic_cast<CCAccelDeccelAmplitude*>(m_pOther)->setAmplitudeRate(powf(f, m_fRate));
	}

	CCIntervalAction* CCAccelDeccelAmplitude::reverse(void)
	{
		return CCAccelDeccelAmplitude::actionWithAction(m_pOther->reverse(), m_fDuration);
	}

	// implementation of AccelAmplitude

	CCAccelAmplitude* CCAccelAmplitude::actionWithAction(cocos2d::CCAction *pAction, cocos2d::ccTime duration)
	{
		CCAccelAmplitude *pRet = new CCAccelAmplitude();
		if (pRet)
		{
			if (pRet->initWithAction(pAction, duration))
			{
				pRet->autorelease();
			}
			else
			{
				CCX_SAFE_DELETE(pRet);
			}
		}

		return pRet;
	}

	bool CCAccelAmplitude::initWithAction(cocos2d::CCAction *pAction, cocos2d::ccTime duration)
	{
		if (__super::initWithDuration(duration))
		{
			m_fRate = 1.0f;
			m_pOther = dynamic_cast<CCIntervalAction*>(pAction);
			pAction->retain();

			return true;
		}

		return false;
	}

	CCAccelAmplitude::~CCAccelAmplitude(void)
	{
		CCX_SAFE_DELETE(m_pOther);
	}

	void CCAccelAmplitude::startWithTarget(CCNode *pTarget)
	{
		__super::startWithTarget(pTarget);
		m_pOther->startWithTarget(pTarget);
	}

	void CCAccelAmplitude::update(cocos2d::ccTime time)
	{
        dynamic_cast<CCAccelAmplitude*>(m_pOther)->setAmplitudeRate(powf(time, m_fRate));
		m_pOther->update(time);
	}

	CCIntervalAction* CCAccelAmplitude::reverse(void)
	{
		return CCAccelAmplitude::actionWithAction(m_pOther->reverse(), m_fDuration);
	}

	// DeccelAmplitude

	CCDeccelAmplitude* CCDeccelAmplitude::actionWithAction(cocos2d::CCAction *pAction, cocos2d::ccTime duration)
	{
		CCDeccelAmplitude *pRet = new CCDeccelAmplitude();
		if (pRet)
		{
			if (pRet->initWithAction(pAction, duration))
			{
				pRet->autorelease();
			}
			else
			{
				CCX_SAFE_DELETE(pRet);
			}
		}

		return pRet;
	}

	bool CCDeccelAmplitude::initWithAction(cocos2d::CCAction *pAction, cocos2d::ccTime duration)
	{
		if (__super::initWithDuration(duration))
		{
			m_fRate = 1.0f;
			m_pOther = dynamic_cast<CCIntervalAction*>(pAction);
			pAction->retain();

			return true;
		}

		return false;
	}

	CCDeccelAmplitude::~CCDeccelAmplitude(void)
	{
		CCX_SAFE_RELEASE(m_pOther);
	}

	void CCDeccelAmplitude::startWithTarget(CCNode *pTarget)
	{
		__super::startWithTarget(pTarget);
		m_pOther->startWithTarget(pTarget);
	}

	void CCDeccelAmplitude::update(cocos2d::ccTime time)
	{
        dynamic_cast<CCDeccelAmplitude*>(m_pOther)->setAmplitudeRate(powf((1 - time), m_fRate));
		m_pOther->update(time);
	}

	CCIntervalAction* CCDeccelAmplitude::reverse(void)
	{
		return CCDeccelAmplitude::actionWithAction(m_pOther->reverse(), m_fDuration);
	}

	// implementation of StopGrid

	void CCStopGrid::startWithTarget(CCNode *pTarget)
	{
		__super::startWithTarget(pTarget);

		CCGridBase *pGrid = dynamic_cast<CCNode*>(m_pTarget)->getGrid();
		if (pGrid && pGrid->isActive())
		{
			pGrid->setActive(false);
		}
	}

	CCStopGrid* CCStopGrid::action(void)
	{
		CCStopGrid* pAction = new CCStopGrid();
		pAction->autorelease();

		return pAction;
	}

	// implementation of CCReuseGrid

	CCReuseGrid* CCReuseGrid::actionWithTimes(int times)
	{
		CCReuseGrid *pAction = new CCReuseGrid();
		if (pAction)
		{
			if (pAction->initWithTimes(times))
			{
				pAction->autorelease();
			}
			else
			{
				CCX_SAFE_DELETE(pAction);
			}
		}

		return pAction;
	}

	bool CCReuseGrid::initWithTimes(int times)
	{
		m_nTimes = times;

		return true;
	}

	void CCReuseGrid::startWithTarget(CCNode *pTarget)
	{
		__super::startWithTarget(pTarget);

		CCNode *myTarget = dynamic_cast<CCNode*>(m_pTarget);
		if (myTarget->getGrid() && myTarget->getGrid()->isActive())
		{
			myTarget->getGrid()->setReuseGrid(myTarget->getGrid()->getReuseGrid() + m_nTimes);
		}
	}
} // end of namespace cocos2d
