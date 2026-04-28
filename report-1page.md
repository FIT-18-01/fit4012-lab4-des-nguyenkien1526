# Report 1 page - Lab 4 DES / TripleDES

## Mục tiêu

Xây dựng chương trình C++ thực hiện DES và TripleDES theo mô hình nhập từ `stdin`, có hỗ trợ mã hóa nhiều block, zero padding, giải mã DES và giải mã TripleDES.

## Cách làm / Method

- Tạo đầy đủ các bảng chuẩn của DES: IP, FP, E, P, PC-1, PC-2 và 8 S-box.
- Sinh 16 round key từ key 64 bit.
- Cài đặt hàm mã hóa một block DES theo cấu trúc Feistel.
- Cài đặt giải mã bằng cách dùng round key theo thứ tự đảo ngược.
- Ghép DES vào TripleDES theo chuỗi EDE: `E(K3, D(K2, E(K1, P)))`.
- Xử lý plaintext dài hơn 64 bit bằng cách tách block và đệm `0` ở cuối block cuối.

## Kết quả / Result

Chương trình chạy được 4 mode:

1. DES encrypt
2. DES decrypt
3. TripleDES encrypt
4. TripleDES decrypt

Các vector kiểm thử của bài đều cho kết quả bit đúng, bao gồm:

- plaintext dài hơn 64 bit trong mode 1
- round-trip encrypt/decrypt với DES
- TripleDES encrypt/decrypt đúng với bộ khóa `K1`, `K2`, `K3`
- test negative cho tamper và wrong key

## Kết luận / Conclusion
Bài lab giúp em hiểu rõ hơn về:

- cách DES chia khối và sinh round key
- vai trò của hoán vị, S-box và Feistel network
- cách TripleDES tái sử dụng DES theo EDE
- giới hạn của zero padding trong thực tế

Nếu làm tiếp, bước cải tiến hợp lý nhất là tách mã nguồn thành nhiều file, thêm kiểm thử tự động chi tiết hơn và chuẩn hóa xử lý padding bằng một scheme có khả năng giải mã an toàn hơn.
