# Giao diện AgentLoop và kiểm thử offline

Header chính nằm trong `include/client/` và `include/agent/`. Các file cùng tên
trong `src/client/` và `src/agent/` chuyển tiếp đến header chính để đáp ứng đường
dẫn đầu ra của dự án mà không nhân đôi định nghĩa.

## Hợp đồng Agent / Harness

- `StepHook = std::function<void(const StepEvent&)>`; Harness đăng ký bằng
  `setStepHook()`. Agent không include hay phụ thuộc Harness.
- Hook chạy đồng bộ một lần cho mỗi bước đã bắt đầu, sau khi có kết quả hoặc
  sau khi xác định lỗi LLM / chặn lặp. Callback cần sao chép event nếu lưu lâu dài.
- `observation`: đầu vào của bước. `decision`: quyết định gọi Tool hoặc trả lời.
- `result`: kết quả Tool, câu trả lời cuối hoặc thông báo lỗi/chặn lặp.
- `duration_ms`: thời gian từ đầu bước đến trước callback, gồm thời gian Tool.
- `status`: `Completed`, `ThinkingFailed`, `LoopBlocked`. Khi suy nghĩ thất bại,
  `decision` chứa `FinalAnswerAction` với thông báo lỗi để giữ tương thích kiểu
  dữ liệu hiện tại; bên nhận phải đọc `status` trước khi đánh giá thành công.
- `thought` để trống vì parser hiện chưa trích xuất trường này từ LLM.
- Hook không nên ném exception. Exception từ callback hoặc hàm override hiện
  truyền về caller; việc ghi event lỗi chỉ áp dụng đường lỗi LLM trả về và chặn lặp.
- `max_steps` là số lượt suy nghĩ tối đa. Tool ở lượt cuối vẫn chạy, sau đó Agent
  báo hết giới hạn nếu chưa có câu trả lời cuối. Ngân sách không dương không chạy bước nào.

## Phạm vi mock và tích hợp tiếp theo

`act()` mặc định vẫn là mock phục vụ thiết kế G1. ToolRegistry hiện thuộc
namespace `edu`, còn Agent thuộc `edu_schedule`. Khi nối Tool thật, cần thống
nhất cách đổi `edu::ToolResult` sang kết quả Agent và cách biểu diễn tham số.
Tài liệu này mô tả giao diện hiện tại, chưa phải xác nhận thống nhất của nhóm.

## Kiểm thử offline

Sau khi cấu hình CMake theo README:

```sh
cmake --build build
ctest --test-dir build --output-on-failure
```

`BUILD_TESTING` mặc định bật. Test AgentLoop không gọi Ollama hay mạng. Sáu kịch bản
kiểm tra trả lời ngay, Tool rồi trả lời, hết giới hạn, lặp và reset, lỗi LLM,
và lặp ping-pong. Test bao gồm kết quả hook và thời gian thực thi Tool.
