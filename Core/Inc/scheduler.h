/*
 * scheduler.h
 *
 * Created on: Oct 30, 2025
 * Author: DuongDepTrai
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>


typedef struct sTask {
	 void (*pTask)(void); // Con trỏ đến hàm của tác vụ
	 uint32_t Delay; // Thời gian trễ (delta-time)
	 uint32_t Period; // Chu kỳ lặp lại
	 uint8_t RunMe; // Cờ báo sẵn sàng chạy (1 = sẵn sàng)
	 uint32_t TaskID; // ID (chính là index trong mảng)

	 struct sTask* next; // Con trỏ đến task tiếp theo trong danh sách
} sTask;

#define SCH_MAX_TASKS 10 // Số tác vụ tối đa
#define NO_TASK_ID SCH_MAX_TASKS

//============ Khai báo hàm ========//

// Khởi tạo bộ lập lịch
void SCH_Init(void);

// Thêm tác vụ
uint32_t SCH_Add_Task(void (*pFunction)(),uint32_t DELAY, uint32_t PERIOD);

// Hàm Update (gọi trong ISR)
void SCH_Update(void);

// Hàm Điều phối (gọi trong while(1))
void SCH_Dispatch_Tasks(void);

// Xóa tác vụ
uint8_t SCH_Delete_Task(uint32_t TaskID);

#endif /* INC_SCHEDULER_H_ */
