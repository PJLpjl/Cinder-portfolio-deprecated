#include "LedMatrixApp.h"
#include "LedState.h"
#include "LedManager.h"

namespace
{
	float time_last_signal_came[N_DEVICES]={0};
	const float sec_turn_to_idle = 5;//change to idle states if no one moving
}

void LedMatrixApp::update()
{
	int win_w = getWindowWidth();
	int win_h = getWindowHeight();
	float elapsed = getElapsedSeconds();

	for (int dev_id=0;dev_id<2;dev_id++)
	{
		bool isIdle = LedState::isIdleState(current_states[dev_id]->_type);
		if (elapsed - time_last_signal_came[dev_id] > sec_turn_to_idle)
		{
			if (isIdle)
			{
				changeToRandomInteractiveState(dev_id);
				isIdle = false;
			}
		}
		{
			centers[dev_id].clear();
			mtx_kinect_queues[dev_id].lock();
			if (kinect_queues[dev_id].empty())
			{//unlock ASAP
				mtx_kinect_queues[dev_id].unlock();
			}
			else
			{//some one is moving before Kinect
				if (!isIdle)
				{
					changeToRandomIdleState(dev_id);
					continue;
				}

				time_last_signal_came[dev_id] = elapsed;

				vector<Vec3f> raw_centers = kinect_queues[dev_id].front();
				kinect_queues[dev_id].pop_front();
				mtx_kinect_queues[dev_id].unlock();//unlock

				int n_raws = raw_centers.size();
				for (int i=0;i<n_raws;i++)
				{
					const Vec3f& ct = raw_centers[i];
					int x = static_cast<int>(ct.x*win_w);
					int y = static_cast<int>(ct.y*win_h);
					int z = (int)lmap<float>(ct.z, 800, 4000, 0, Z-1);
					centers[dev_id].push_back(Vec3i(x,y,z));
				}
				//send to interactive state
				//for [centers] no mutex is needed
			}
		}
		LedManager::get(dev_id).reset();
		current_states[dev_id]->update();
	}
}
