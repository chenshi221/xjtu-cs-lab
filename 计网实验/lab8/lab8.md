# 类qq通信协议说明

1. login：

    ```mermaid
    sequenceDiagram
    participant A as 客户端
    participant B as 服务器
    participant C as 其他客户端
    A->>B: json{"message_type":"login","username":
    A->>B:"xxx","password":"xxx"}
    B->>A: json{"message_type":"login",
    B->>A:"content":"success"}
    B->>C: json{"message_type":"update_online_user"
    B->>C:,"online_user":["xxx"]}
    
    ```

2. register：

    ```mermaid
    sequenceDiagram
    participant A as 客户端
    participant B as 服务器
    A->>B: json{"message_type":"register",
    A->>B:"username":"xxx","password":"xxx"}
    B->>A: json{"message_type":"register","content":"success"}
    ```

3. offline：

    ```mermaid
    sequenceDiagram
    participant A as 客户端
    participant B as 服务器
    participant C as 其他客户端
    A->>B: json{"message_type":"leave_message"
    A->>B:,"username":"xxx"}
    B->>C: json{"message_type":"leave_message"
    B->>C:,"username":"xxx"}
    ```

4. private chat：

    ```mermaid
    sequenceDiagram
    participant A as 聊天发起者
    participant B as 服务器
    participant C as 聊天接收者
    A->>B: json{"message_type":"word","chat_type":"private",
    A->>B:"send_user":"xxx","rece_user":"xxx","content":"xxx"}
    B->>C: json{"message_type":"word","chat_type":"private",
    B->>C:"send_user":"xxx","rece_user":"xxx","content":"xxx"}
    ```

5. normal chat：

    ```mermaid
    sequenceDiagram
    participant A as 聊天发起者
    participant B as 服务器
    participant C as 聊天接收者
    A->>B: json{"message_type":"word","chat_type":"normal",
    A->>B:"send_user":"xxx","rece_user":"xxx","content":"xxx"}
    B->>C: json{"message_type":"word","chat_type":"normal",
    B->>C:"send_user":"xxx","rece_user":"xxx","content":"xxx"}
    ```

6. send file request：

    ```mermaid
    sequenceDiagram
    participant A as 文件发送者
    participant B as 服务器
    participant C as 文件接收者
    A->>B: json{"message_type":"file","send_user":"xxx",
    A->>B:"rece_user":"xxx","file_name":"xxx","file_length":xxx}
    A->>B: file data
    B->>C: json{"message_type":"file","send_user":"xxx",
    B->>C:"file_name":"xxx","file_length":xxx}
    ```

7. accept file request：

    ```mermaid
    sequenceDiagram
    participant A as 文件接收者
    participant B as 服务器
    A->>B: json{"message_type":"file","content":"accept",
    A->>B:"file_name":"xxx","file_length":xxx,"send_user":"xxx"}
    B->>A: json{"message_type":"file-data","file_name":"xxx",
    B->>A:"file_length":xxx,"send_user":"xxx"}
    B->>A: file data
    ```

8. reject file request：

    ```mermaid
    sequenceDiagram
    participant A as 文件接收者
    participant B as 服务器
    A->>B: json{"message_type":"reject","file_name":"xxx"}
    ```

9. voice chat：

    ```mermaid
    sequenceDiagram
    participant A as 聊天发起者
    participant B as 服务器
    participant C as 聊天接收者
    A->>B: json{"message_type":"voice","content":"request",
    A->>B:"send_user":"xxx","rece_user":"xxx"
    B->>C: json{"message_type":"voice","content":"request",
    B->>C:"send_user":"xxx","rece_user":"xxx"
    B->>C:"other_ip":"xxx","other_port":10010
    B->>C:"your_ip":"xxx","your_port":10011}
    C->>B: json{"message_type":"voice","content":"accept",
    C->>B:"send_user":"xxx","rece_user":"xxx"}
    B->>A: json{"message_type":"voice","content":"accept",
    B->>A:"send_user":"xxx","rece_user":"xxx"
    B->>A:"other_ip":"xxx","other_port":10011
    B->>A:"your_ip":"xxx","your_port":10010}
    A-->C: voice data BY UDP
    A->>B: json{"message_type":"voice","content":"close"}
    B->>C: json{"message_type":"voice","content":"close"}
    ```

