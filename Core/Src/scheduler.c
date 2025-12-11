/*
 * scheduler.c
 *
 * Created on: Oct 30, 2025
 * Author: DuongDepTrai
 *
 * Phiên bản O(1) Update - Sử dụng Sorted Delta Linked List
 */
#include "scheduler.h"

// Mảng chứa các task (đóng vai trò là memory pool)
sTask SCH_tasks_G[SCH_MAX_TASKS];

// Con trỏ đến đầu danh sách (task sắp chạy nhất)
sTask* SCH_list_head = 0;


/**
 * @brief (Hàm nội bộ) Chèn một task vào danh sách liên kết
 * theo thứ tự sắp xếp của Delay (delta-time)
 * @param taskToInsert: Con trỏ đến task trong mảng SCH_tasks_G
 * @note Đây là thao tác O(n)
 */
void SCH_Insert_Task_Sorted(sTask* taskToInsert) {
    // Nếu danh sách rỗng, chèn vào đầu
    if (SCH_list_head == 0) {
        SCH_list_head = taskToInsert;
        taskToInsert->next = 0;
        return;
    }

    // Nếu task mới cần chạy sớm hơn cả head
    if (taskToInsert->Delay < SCH_list_head->Delay) {
        // Cập nhật delta-time của head cũ
        SCH_list_head->Delay -= taskToInsert->Delay;

        // Chèn task mới làm head
        taskToInsert->next = SCH_list_head;
        SCH_list_head = taskToInsert;
        return;
    }

    // Duyệt danh sách để tìm vị trí chèn
    sTask* current = SCH_list_head;
    //sTask* prev = SCH_list_head;

    // Trừ đi delta-time của các task trên đường đi
    taskToInsert->Delay -= current->Delay;

    // Tìm vị trí: current != 0 VÀ task mới có delay > delay của task 'current'
    while ((current->next != 0) && (taskToInsert->Delay > current->next->Delay)) {
        current = current->next;
        taskToInsert->Delay -= current->Delay;
    }

    // Đã tìm được vị trí (chèn sau 'current')
    if (current->next != 0) {
        // Cập nhật delta-time của task kế tiếp
        current->next->Delay -= taskToInsert->Delay;
    }

    // Chèn task mới vào danh sách
    taskToInsert->next = current->next;
    current->next = taskToInsert;
}

/**
 * @brief Khởi tạo bộ lập lịch
 */
void SCH_Init(void) {
    unsigned char i;
    for (i = 0; i < SCH_MAX_TASKS; i++) {
        // Xóa tất cả tác vụ (giải phóng)
        SCH_tasks_G[i].pTask = 0;
        SCH_tasks_G[i].next = 0;
        SCH_tasks_G[i].RunMe = 0;
        SCH_tasks_G[i].TaskID = i;
    }
    SCH_list_head = 0; // Danh sách rỗng
}

/**
 * @brief Thêm một tác vụ mới vào bộ lập lịch
 * @return TaskID (index) hoặc NO_TASK_ID nếu lỗi
 */
uint32_t SCH_Add_Task(void (* pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
    unsigned char Index = 0;

    // 1. Tìm một slot trống trong mảng
    while ((SCH_tasks_G[Index].pTask != 0) && (Index < SCH_MAX_TASKS)) {
        Index++;
    }

    // 2. Nếu mảng đầy, trả về lỗi
    if (Index == SCH_MAX_TASKS) {

        return NO_TASK_ID;
    }

    // 3. Nếu tìm thấy, gán thông tin cơ bản
    SCH_tasks_G[Index].pTask = pFunction;
    SCH_tasks_G[Index].Period = PERIOD;
    SCH_tasks_G[Index].RunMe = 0;
    SCH_tasks_G[Index].next = 0;
    // Gán Delay (sẽ được dùng để sắp xếp)
    SCH_tasks_G[Index].Delay = DELAY;

    // 4. Chèn vào danh sách liên kết đã sắp xếp
    SCH_Insert_Task_Sorted(&SCH_tasks_G[Index]);

    // 5. Trả về ID
    return Index;
}

/**
 * @brief Xóa một tác vụ
 * @param taskID: Vị trí (index) của tác vụ
 * @return 1 nếu thành công, 0 nếu thất bại
 */
uint8_t SCH_Delete_Task(uint32_t taskID) {
    if (taskID >= SCH_MAX_TASKS || SCH_tasks_G[taskID].pTask == 0) {
        return 0; // ID không hợp lệ hoặc slot đã trống
    }

    // 1. Nếu task đang ở đầu danh sách
    if (SCH_list_head == &SCH_tasks_G[taskID]) {
        // Cập nhật delta-time cho task kế tiếp (nếu có)
        if (SCH_list_head->next != 0) {
            SCH_list_head->next->Delay += SCH_list_head->Delay;
        }
        SCH_list_head = SCH_list_head->next;
    }
    // 2. Nếu task ở giữa hoặc cuối danh sách (O(n) search)
    else {
        sTask* current = SCH_list_head;
        while (current != 0 && current->next != &SCH_tasks_G[taskID]) {
            current = current->next;
        }

        // Nếu tìm thấy task (current != 0)
        if (current != 0) {
            sTask* taskToDel = current->next;
            // Cập nhật delta-time cho task kế tiếp (nếu có)
            if (taskToDel->next != 0) {
                taskToDel->next->Delay += taskToDel->Delay;
            }
            // Bỏ qua taskToDel khỏi danh sách
            current->next = taskToDel->next;
        }
    }

    // 3. Giải phóng task trong mảng (đánh dấu là trống)
    SCH_tasks_G[taskID].pTask = 0;
    SCH_tasks_G[taskID].next = 0;
    SCH_tasks_G[taskID].Period = 0;
    SCH_tasks_G[taskID].Delay = 0;
    SCH_tasks_G[taskID].RunMe = 0;

    return 1;
}

/**
 * @brief Hàm cập nhật bộ đếm (gọi bởi ISR Timer)

 */
void SCH_Update(void) {
    // Nếu có task trong danh sách
    if (SCH_list_head != 0) {
        // Giảm delay của task đầu tiên
    	if(SCH_list_head->Delay > 0){
    		  SCH_list_head->Delay--;
    	}


        // Nếu task đầu tiên đã đến lúc
        while (SCH_list_head != 0 && SCH_list_head->Delay == 0) {
            // Đặt cờ RunMe
            SCH_list_head->RunMe = 1;

            // Lấy task ra khỏi danh sách
            sTask* taskToRun = SCH_list_head;
            SCH_list_head = SCH_list_head->next;
            taskToRun->next = 0; // Ngắt kết nối
        }
    }
}

/**
 * @brief Hàm điều phối tác vụ (gọi trong while(1))
 */
void SCH_Dispatch_Tasks(void) {
    unsigned char Index;

    // Duyệt qua mảng task (bể chứa)
    for (Index = 0; Index < SCH_MAX_TASKS; Index++) {
        // Nếu task này có cờ RunMe
        if (SCH_tasks_G[Index].RunMe > 0) {
            // 1. Chạy tác vụ
            (*SCH_tasks_G[Index].pTask)();

            // 2. Reset cờ
            SCH_tasks_G[Index].RunMe = 0;

            // 3. Nếu là tác vụ định kỳ
            if (SCH_tasks_G[Index].Period > 0) {
                // Đặt lại delay và thêm lại vào danh sách
                SCH_tasks_G[Index].Delay = SCH_tasks_G[Index].Period;
                SCH_Insert_Task_Sorted(&SCH_tasks_G[Index]);
            }
            // 4. Nếu là tác vụ 1 lần, giải phóng nó
            else {
                SCH_tasks_G[Index].pTask = 0;
            }
        }
    }
}
