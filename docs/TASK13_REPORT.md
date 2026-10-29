# TASK-13 — Audit, triển khai và kết quả kiểm thử

Ngày kiểm tra: 29/10/2026 (Asia/Saigon). Checkout gốc: `1a9fd18`.

**Trạng thái: hoàn thành parser và kiểm thử offline độc lập; chưa nghiệm thu toàn bộ
TASK-13.** Còn thiếu kết quả Ollama/Kaggle thật và quyết định contract native tool
calling. Không commit, push, merge hoặc cập nhật bảng tiến độ của nhóm.

## 1. Audit chỉ đọc và quyền sở hữu

`git status --short` chạy đầu tiên tại workspace cha báo không phải repository;
chạy lại tại `AI-Agent/` cho kết quả rỗng. Đã đọc client headers/implementation,
Tool, ToolRegistry, AgentLoop, main, CMake, toàn bộ các test hiện có và tài liệu
trong `docs/`. Đã xem trực tiếp `assets/timetab# TASK-13 — EduSchedule-Agent (C++23)

Bạn là Senior C++ Engineer, hỗ trợ tôi hoàn thành TASK-13: Multimodal Base64 đọc ảnh thời khóa biểu và JSON Tool Parser.

**Repository:** https://github.com/vhnam1408/AI-Agent

**Yêu cầu:** Tuân thủ kế hoạch TASK-13 và đề đồ án OOP 2026. Không được làm ảnh hưởng đến code của thành viên khác.

## 1. Kiểm tra trước khi viết

Đọc code hiện tại, đặc biệt:

- `include/client/llm_client.h`
- `include/client/ollama_client.h`
- `src/client/ollama_client.cpp`
- `src/agent/Tool.h`
- `src/agent/ToolRegistry.h`
- `src/main.cpp`
- `CMakeLists.txt`
- `assets/timetable.png`
- Các test hiện có trong `tests/`

Dự án đã có Base64, HTTP `/api/chat`, xử lý lỗi và CLI gửi ảnh. **Không viết lại chức năng đã hoàn thành.**

Đặc biệt kiểm tra `OllamaClient::parse_ollama_response()`: hiện phần response chủ yếu lấy `message.content`, chưa có luồng xử lý đầy đủ cho `message.tool_calls`.

## 2. Quy tắc bảo vệ source code

- Bắt đầu bằng READ-ONLY AUDIT và `git status --short`.
- Dùng lịch sử commit và tài liệu nhóm để xác minh quyền sở hữu file.
- Chỉ được sửa file xác nhận do tôi phụ trách hoặc tạo file mới riêng cho TASK-13.
- Không tự sửa các module ToolRegistry, AgentLoop, Scheduler, Harness và interface dùng chung thuộc người khác.
- Không sửa `CMakeLists.txt`, `src/main.cpp` hoặc shared headers khi chưa xác minh quyền sở hữu và được phép.
- Không đổi public API của thành viên khác.
- Không chạy các lệnh Git có thể làm mất code; không tự commit, push hoặc merge.
- Nếu phải thay đổi contract chung, chỉ đề xuất giải pháp và dừng tại đó để nhóm xem xét.

## 3. Triển khai TASK-13

Ưu tiên ba phần:

**A. Multimodal:** Tái sử dụng chức năng mã hóa Base64 và gửi ảnh hiện có. Bổ sung tối thiểu những gì cần để đọc ảnh thời khóa biểu và nhận dữ liệu AI.

**B. JSON Tool Parser:** Phân tích cả JSON trong `message.content` và native `message.tool_calls`. Kiểm tra tên hàm, arguments và lỗi dữ liệu. Tuân thủ contract TASK-06; parser không được tự thực thi Tool.

**C. Test:** Có mock JSON offline, test parser C++, test ảnh `assets/timetable.png`, test với Ollama thật và xác minh dữ liệu thời khóa biểu có chính xác không.

Sử dụng C++23 theo cấu hình CMake hiện tại. Không hạ chuẩn C++ hoặc làm hỏng các test cũ.

## 4. Kiểm thử trên Kaggle

Tôi có notebook `AI_Agent_Task13_Kaggle.ipynb` để:

- Clone repository.
- Build C++23 và chạy CTest.
- Cài Ollama.
- Chạy model `qwen3-vl:8b`, hoặc `gemma4:e4b` / `qwen3.8:27b` khi phù hợp GPU.
- Test text, image và Tool Calling.
- Lưu kết quả JSON.

Không được coi một bài test Python HTTP thành công là bằng chứng parser C++ đã hoàn thành.

## 5. Kết quả mong muốn

Trình bày bằng tiếng Việt:

1. File nào đã có chức năng cần thiết.
2. File nào còn thiếu chức năng.
3. File nào được phép sửa và lý do.
4. Phương án triển khai ít ảnh hưởng nhất.
5. Code và unit test cho TASK-13.
6. Hướng dẫn build và chạy Kaggle.
7. Kết quả PASS/FAIL/SKIP thực tế.
8. Báo cáo tiến độ TASK-13.

**BẮT ĐẦU BẰNG VIỆC KHẢO SÁT REPOSITORY. KHÔNG SỬA CODE TRƯỚC KHI XÁC MINH PHẠM VI VÀ QUYỀN SỞ HỮU FILE.**le.png`.

Nguồn phân công được đọc qua Google Drive, không chỉ suy luận từ Git:

- [Bảng kế hoạch được docs/agent_contract.md dẫn tới](https://docs.google.com/spreadsheets/d/1D1W44CMZDrowgiCz07pfPnjUgCwxdoTfitrn-15sIZ8/edit): TASK-01/05/09/13 thuộc Võ Hoàng Nam; TASK-06 thuộc Trần Hồng Anh; TASK-07/11 thuộc Trương Hoàng Huy; TASK-08 thuộc Đỗ Hồng Ân.
- [Bảng tiến độ mới hơn](https://docs.google.com/spreadsheets/d/1wr1-VLsDf7ww94ATbir1B8VA1_vkPdqlL3m5Hn_nuPs/edit): vẫn phân TASK-13 cho Nam, tuần 1 từ 29/10 đến mốc 01/11/2026. Không dùng hạn 25/10 của bảng cũ để kết luận trễ tiến độ.
- [Kế hoạch chi tiết](https://docs.google.com/document/d/1OwtiAlzU_9VKRn_oe7QrGjzbNe-rZrStS7s80JXiZos/edit), mục 4.2: dữ liệu LLM cần Validator trước khi chuyển sang module khác.
- [PROJECT PLAN](https://docs.google.com/document/d/13MIDFlSODZJrf_NfKdXYANABvO1agqcd6jnQW-R_81w/edit): đã đối chiếu phạm vi TASK-13/G2. Đã đọc mục VI của bản đề OOP 2026 người dùng cung cấp để sắp xếp lại thư mục; chưa nghiệm thu mọi tiêu chí của toàn đồ án.

Lịch sử `867d098` (TASK-09), `0144521` (TASK-05), `81aa98d`/`6833fe5`
(TASK-01) ghi tác giả Võ Hoàng Nam cho client, main, build và test client;
`2eff030` ghi TASK-06 của Ririlaa/Trần Hồng Anh; `1a9fd18` ghi Harness của Ân.
Lịch sử dùng để đối chiếu, không tự coi tác giả commit là quyền sửa mọi contract.

| File/phạm vi | Hiện trạng | Quyết định trong lần này |
|---|---|---|
| `include/client/llm_client.h` | ChatMessage ảnh, API load Base64, expected/error, LLMResponse chỉ có content | Shared contract: đọc, giữ nguyên |
| `include/client/ollama_client.h` | HTTP/parse helpers private; chưa có API gửi tool schema | Đọc, giữ nguyên |
| `src/client/ollama_client.cpp` | Base64, `/api/chat`, ảnh, lỗi, metadata đã có; dòng lấy `message.content` bỏ native calls | Thuộc Nam nhưng tích hợp native cần contract; giữ nguyên |
| `src/main.cpp` | CLI text/image, cấu hình, lưu JSON đã có | Thuộc Nam theo lịch sử, không cần sửa |
| `CMakeLists.txt` gốc | C++23, CURL, 3 test CTest đã có | Thuộc TASK-05/Nam, bổ sung target parser/test theo yêu cầu sắp xếp lại; giữ nguyên target cũ |
| `Tool.h`, `ToolRegistry.h` | ITool nhận string_view; lookup/allow/deny/execute đã có | TASK-06/Anh: giữ nguyên |
| AgentLoop, Scheduler, Harness | Dùng chung; Agent parser cũ đọc Action và gán `{}` | Không sửa; không tích hợp parser vào vòng lặp trong lần này |
| Parser trong client/, test trong tests/, JSON trong assets/, báo cáo trong docs/ | File mới riêng TASK-13 | Được tạo theo phạm vi người dùng cho phép |

File chức năng đều được tạo mới; CMake gốc được bổ sung đăng ký build/test theo yêu cầu sắp xếp lại. Không viết lại Base64, HTTP client hoặc CLI chính.

## 2. Code mới và contract parser

- `include/client/tool_parser.h`, `src/client/tool_parser.cpp`: C++23
  `std::expected<ParsedMessage, ParseError>`. Tái sử dụng `ToolCallAction` hiện có.
- `parse_chat_response(body)`: đọc envelope Ollama, `message.content` và native
  `message.tool_calls`. Giữ nguyên thứ tự nhiều calls. Chấp nhận native thiếu
  content nếu có tool_calls. Văn bản trả lời thường và JSON dữ liệu không chứa
  call được giữ làm content, không biến thành lệnh.
- `parse_tool_content(content)`: định dạng JSON content của TASK-13 như dưới đây;
  nhận object, mảng calls hoặc wrapper native `tool_calls`, hỗ trợ một JSON fence
  hoàn chỉnh. Không dùng regex tìm tùy tiện JSON giữa văn bản.

```json
{"tool_name":"task_lookup","arguments":{"task_id":"T1"}}
```

```json
{"message":{"role":"assistant","content":"","tool_calls":[
  {"function":{"name":"task_lookup","arguments":{"task_id":"T1"}}}
]},"done":true}
```

Arguments phải là object hoặc chuỗi JSON mã hóa object; parser chuẩn hóa thành
chuỗi JSON sở hữu dữ liệu. Từ chối tên sai kiểu/rỗng/sai cú pháp, arguments thiếu,
null, array, primitive hoặc JSON hỏng; từ chối batch có bất kỳ call hỏng, phản hồi
truncated/incomplete và trường hợp cùng chứa calls native lẫn calls trong content.
Caller có thể truyền danh sách tên tool đã biết để bắt UnknownTool. Danh sách
rỗng chỉ kiểm tra cú pháp, không có nghĩa được phép thực thi.

Parser không giữ Registry, không gọi execute, không sửa policy, không lưu lịch.
Đã test bằng CountingTool và Registry bị deny. TASK-06 chỉ quy định input
`string_view`, chưa quy định JSON schema nghiệp vụ cho mọi tool. Vì vậy kiểm tra
required fields/type nghiệp vụ từng tool phải dùng schema thống nhất với chủ tool;
parser hiện chỉ bảo đảm arguments là object hợp lệ, không nhận là JSON Schema validator tổng quát.

**Chú ý tích hợp:** TaskLookupTool hiện nhận ID thô `T1`, CalculatorTool nhận biểu
thức thô; không đưa thẳng `{"task_id":"T1"}` vào các tool ấy rồi báo tích hợp đúng.
Cần adapter theo từng tool được chủ module duyệt. Các tên/schema trong fixture
là mock cho parser, không sửa contract execute của tool thật.

## 3. Những điểm phải nhóm duyệt trước khi nối native end-to-end

Không có code thay đổi contract trong lần này. Đề xuất để review:

1. Thêm kiểu dữ liệu call ở tầng client (không phụ thuộc Agent), giữ name,
   arguments JSON object và ID nếu backend có; thêm vector calls vào LLMResponse.
   Không nhét envelope vào `content`, vì sẽ đổi ý nghĩa dữ liệu hiện có.
2. Chốt cách cung cấp tool schemas cho request Ollama (hiện build_payload chưa có
   `tools`). Không tự đổi virtual `LLMClient::chat()` khi module khác còn phụ thuộc.
3. Chốt xử lý batch nhiều calls: AgentDecision hiện chỉ biểu diễn một action.
   Không bỏ các call sau hoặc tự chạy chúng trong parser.
4. Chốt schema/adapter arguments với TASK-06, rồi Agent mới dispatch qua
   Registry::execute và policy hiện có. Chỉ cho phép đọc tên tool không thay thế
   confirm khi ghi dữ liệu.

Do giới hạn bảo vệ source của người dùng, phần này dừng ở đề xuất. Native raw
response có thể kiểm thử parser độc lập ngay, nhưng OllamaClient -> AgentLoop
native end-to-end **chưa hoàn thành**.

## 4. Test và kết quả thực tế

Môi trường: Windows, MSYS2 UCRT64 GCC 16.1.0, C++23, libcurl 8.20.0,
Python 3.14.5, Release. Parser được compile với
`-Wall -Wextra -Wpedantic -Werror`; checks không dùng assert bị tắt bởi NDEBUG.

| Kiểm tra đã thực hiện | Kết quả |
|---|---|
| Root CMake build trước khi thêm code | PASS |
| 3 test CTest gốc trước khi thêm code | PASS 3/3 |
| Build riêng TASK-13, kèm các target gốc | PASS |
| task13_parser | PASS, 55 checks: fixtures native/content, nhiều calls, object/string args, lỗi, final answers, policy/no execution |
| task13_image | PASS, đọc PNG thật bằng Base64 có sẵn |
| task13_transport | PASS, giải mã Base64 đối chiếu toàn bộ byte ảnh; gửi qua client C++ thật tới HTTP mock; content nhận qua parser C++ |
| agent_contract, ollama_client, ollama_http trong build mới | PASS 3/3 |
| task13_live | SKIP: chưa bật TASK13_LIVE, không có phiên Ollama/Kaggle/model được cung cấp |
| tests/calculator_tool_test.cpp ngoài target CTest | FAIL compile có sẵn: setToolAccess không tồn tại, ToolAccess chưa khai báo; không sửa file/Registry của người khác |
| tests/harness_contract_test.cpp, tests/task01_ollama_smoke_test.cpp | PASS syntax-only; chưa chạy runtime; không tính là test hành vi PASS |

CTest mới thực tế: **6 PASS, 0 FAIL, 1 SKIP**. Lỗi calculator ở hàng riêng
không được root CMake đăng ký; do đó không suy từ CTest xanh rằng mọi file test
đều hoạt động. Không sửa test calculator hay Registry; lỗi compile đó không do TASK-13 gây ra.

Ground truth `assets/timetable_expected.json` được chép từ ảnh đã
xem: 9 buổi (thứ 2: 2; thứ 3: 2; thứ 4: 3; thứ 7: 2). Không có nhãn giờ/ngày
cụ thể trong ảnh; start/end là null, không suy đoán tiết học từ vị trí ô.
Ground truth không được gửi trong prompt. Offline mock chỉ chứng minh luồng dữ
liệu, không chứng minh model đọc đúng ảnh.

Live test so sánh multiset đầy đủ các hàng (thứ/LT-TH/môn/phòng), chuẩn hóa NFC và
khoảng trắng, không tự bỏ dấu hay sửa tên môn để ép PASS. Bắt thiếu, thừa và trùng
buổi; bắt giờ model tự bịa; lưu precision/recall và raw/result JSON. Chưa có số
liệu OCR thực tế trong lần này.

## 5. Build và chạy

Chạy từ repository root trong môi trường có compiler C++23 và libcurl:

```sh
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build -j 2
ctest --test-dir build --output-on-failure
```

MSYS2 UCRT64 trên Windows có thể dùng:

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=C:/msys64/ucrt64/bin/mingw32-make.exe -DBUILD_TESTING=ON
cmake --build build -j 4
ctest --test-dir build --output-on-failure
```

Giữ UCRT64/bin trên PATH để tìm DLL. Build trực tiếp từ CMake gốc, bổ sung parser và test mới; các target cũ và chuẩn C++23 được giữ nguyên.

Thử trực tiếp parser với mock native (Windows thêm `.exe`):

```sh
build_probe parse assets/tool_parser_native.json
```

Chạy Ollama thật trên Linux/Kaggle sau khi server sẵn sàng và model đã pull:

```sh
TASK13_LIVE=1 TASK13_MODEL=qwen3-vl:8b TASK13_URL=http://127.0.0.1:11434 \
  ctest --test-dir build -R '^task13_live$' -V --output-on-failure
```

Trên PowerShell đặt ba biến tương ứng bằng `$env:TASK13_LIVE='1'`,
`$env:TASK13_MODEL='qwen3-vl:8b'`, `$env:TASK13_URL='http://127.0.0.1:11434'`.
Khi đã bật live nhưng thiếu model/server hoặc model sai kết quả, test FAIL, không
âm thầm đổi thành SKIP.

## 6. Kaggle

Không lưu notebook trong repository theo yêu cầu người dùng. Có thể chạy các
lệnh build và live test ở mục 5 trong terminal của Kaggle.

1. Bật Internet/GPU; chọn REF có code TASK-13 đã được nhóm
   đưa lên remote, hoặc upload checkout vào `/kaggle/working/AI-Agent`.
2. Kiểm tra checkout có các file parser/test mới trước khi build; không dùng kết quả
   test của main cũ để báo TASK-13 PASS. Không tự commit hoặc push.
3. Cài dependency, kiểm tra `std::expected`, build C++23 và chạy CTest offline.
   Nếu image không có package g++-13, cần dùng image/toolchain repo phù hợp;
   không giảm standard để lách lỗi compiler.
4. Cài Ollama từ script chính thức, chờ `/api/version`, chọn/pull model và xem
   `ollama show`, `nvidia-smi`. Bắt đầu với qwen3-vl:8b; đổi model theo VRAM.
5. Chạy live CTest. Text và native response được executable C++ parse; ảnh đi qua
   Base64 + OllamaClient C++ hiện có, sau đó parser C++. Python chỉ điều khiển
   request native tool schema và so sánh kết quả; HTTP 200 riêng không đủ PASS.
6. Tải thư mục results trong build directory, gồm report.json, text/native raw JSON,
   output parser C++, kết quả ảnh và stderr. Kiểm tra ollama ps để xác nhận GPU thực tế.

Theo model library đã kiểm tra: [qwen3-vl:8b](https://ollama.com/library/qwen3-vl)
weights khoảng 6.2 GB, [gemma4:e4b](https://ollama.com/library/gemma4) khoảng
6.6–9.5 GB, [qwen3.8:27b](https://ollama.com/library/qwen3.8) khoảng 18 GB.
Còn cần VRAM cho context/vision/runtime; không coi hai GPU 16 GB là tự động có
một vùng VRAM 32 GB. Native tools là trường riêng của
[/api/chat](https://docs.ollama.com/api/chat), không nằm trong content.

## 7. Tiến độ để báo nhóm

Đề xuất trạng thái **Doing — offline verified, live và shared contract pending**.
Đã có parser C++23, fixture mock, test ảnh/transport và lệnh nghiệm thu.
Giữ nguyên toàn bộ source/interface của các thành viên khác. Cần nhóm chốt các
điểm ở mục 3 và chạy live test thật trước khi chuyển TASK-13 sang Done.


## 8. Cập nhật cấu trúc theo yêu cầu người dùng

Đã bỏ các thư mục riêng theo task. Parser nằm trong client/, test nằm trực tiếp tests/, mock JSON ở assets/. Notebook đã được xóa theo yêu cầu người dùng. Parser dùng namespace edu_schedule thống nhất với dự án; mỗi hàm trong tool_parser.cpp có chú thích mục đích. CMake gốc được bổ sung target mới, không thay đổi interface chung. Các thư mục build/ là đầu ra sinh tự động, không phải source để nộp.
