#include "analysis.h"
#include <string.h>


static  all_info_typedef  all_info;
static  uchar last_unit;    /* 保留上一个单元信息 */

/**
 * @brief 在线设备数量
 * 
 * @return uchar 
 */
static uchar get_onlineDev(void)
{
    uchar i = 0;
	uchar online_num = 0;
	uchar zero[4] = {0};
	for (i=0; i<NUM_KNEE; i++)
	{
		if (memcmp(all_info.kneeData[i].deviceID,zero,4) && memcmp(all_info.kneeData[i].userID,zero,4)) 
            online_num++;
	}
	return online_num;
}

/**
 * @brief 单元解析
 * 
 * @param pad_data 
 * @return uchar 
 */
uchar set_elementTime(pad_data_typedef* pad_data)
{
	if (pad_data->element > 0 && pad_data->element <= 6)
	{
		if (get_onlineDev()==0) 
        {
            // timeFlag.gatewayStatus = 0;
        }
		// if(timeFlag.gatewayStatus != 1)   
        // {
        //     timeFlag.gatewayStatus = 1;
        // }
		if (last_unit != pad_data->element)   //切换单元
		{
			uchar zero[4] = {0};
			switch(pad_data->element)
			{   //单元时间戳赋值
				case 1:

					if (!memcmp(&all_info.timestamp.classStartTime,zero,4))
					{
						memcpy(&all_info.timestamp.classStartTime,pad_data->UTC,4);
					}
					break;
				case 2:
					if (last_unit == 1)
					{
						memcpy(&all_info.timestamp.unit1EndTime,pad_data->UTC,4);
					}
					break;
				case 3:
					if (last_unit == 2)
					{
						memcpy(&all_info.timestamp.unit2EndTime,pad_data->UTC,4);
					}
					break;
				case 4:
					if (last_unit == 3)
					{
						memcpy(&all_info.timestamp.unit3EndTime,pad_data->UTC,4);
					}
					break;
				case 5:
					if (last_unit == 4)
					{
						memcpy(&all_info.timestamp.unit4EndTime,pad_data->UTC,4);
					}
					break;
				case 6:
					if (last_unit == 5)
					{
						memcpy(&all_info.timestamp.unit5EndTime,pad_data->UTC,4);
					}
					break;
				default:
					break;
			}
			last_unit = pad_data->element;
		}
	}
}
/**
 * @brief	获取一个可绑定的节点号
 * @param	knee：绑定关系存储结构体
 * @return	节点号(即数组位置+1)
 */
uchar get_node_mark(pad_data_typedef *knee)
{
	uchar i;
	uchar zero[9] = {0};
	for (i=0; i<NUM_KNEE; i++)
	{
		if (memcmp(all_info.kneeData[i].classID, knee->classID, 8) && i==0)//切换课程清空所有信息
		{
			// db_Clear_All(dataBlockMain);				
			return i+1;
		}
		else if (!memcmp(all_info.kneeData[i].userID, knee->userID, 5))//用户ID和位置标识均相等,则覆盖重绑用户
		{
			return i+1;
		}
		else if (!memcmp(all_info.kneeData[i].userID, zero, 9))   //用户ID和位置标识 设备ID均为0可以绑定
		{
			return i+1;
		}
	}
	
	return i;
}
/**
 * @brief	向数据块中写入绑定关系
 * @param	绑定关系存储结构体
 * @param	写入到数据块的位置，即节点号-1
 * @return	
 */
uchar set_bingding(pad_data_typedef *knee)
{
	uchar num = knee->node_mark-1;
	uchar i;
	
	for (i=0; i<NUM_KNEE; i++)
	{
		if (!memcmp(all_info.kneeData[i].deviceID,knee->deviceID,4))
		{
			DEBUG_INFO("del:classID = %d,",all_info.kneeData[i].classID[7]);
			DEBUG_INFO("userID = %d,",all_info.kneeData[i].userID[3]);
			DEBUG_INFO("locaFlag = %d,",all_info.kneeData[i].locaFlag);
			DEBUG_INFO("deviceID = %d,",all_info.kneeData[i].deviceID[3]);
			DEBUG_INFO("nodeMark = %d\r\n",i+1);
			memset(all_info.kneeData[i].userID,0,9);
		}
	}
	
	DEBUG_INFO("add:classID = %d,userID = %d,locaFlag = %d,deviceID = %x,nodeMark = %d\r\n",knee->classID[7],knee->userID[3],knee->locaFlag,knee->deviceID[3],knee->node_mark);
	memcpy(all_info.kneeData[num].classID,knee->classID,8); 		
	memcpy(all_info.kneeData[num].userID,knee->userID,4);   	
	all_info.kneeData[num].locaFlag = knee->locaFlag;        	
	memcpy(all_info.kneeData[num].deviceID,knee->deviceID,4);	
#if		!COLLECT_DATA				
	// all_info->kneeData[num].fillFlag = knee->fillFlag;	       
	// all_info->kneeData[num].sendFlag = knee->sendFlag;	
#endif
	
	return 0;
}

