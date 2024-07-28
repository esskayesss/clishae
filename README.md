# clishae
clishae is a simple pastebin server implemented from scratch in c.

### Building the Server

To build the server executable, use the following command:
```sh
make bin
```

### Running the Server

Once built, launch the server by running the executable:
```sh
./bin/server
```

### Usage Examples

1. **Uploading a File**:
   Upload the contents of a file to the server.
   ```sh
   curl --data-binary @Makefile http://localhost:8080/paste
   ```
   Response:
   ```
   http://localhost:8080/raw/1722133682
   ```

2. **Deleting a Paste**:
   Delete a previously uploaded paste using its ID.
   ```sh
   curl -X DELETE http://localhost:8080/paste/1722133682
   ```
   Response:
   ```
   paste was deleted successfully
   ```

3. **Creating a Paste from Text**:
   Create a new paste from text data.
   ```sh
   curl -v localhost:8080/paste -d "clishae works
   hello world"
   ```
   Response (Verbose):
   ```
   * Host localhost:8080 was resolved.
   * IPv6: ::1
   * IPv4: 127.0.0.1
   *   Trying [::1]:8080...
   * connect to ::1 port 8080 from ::1 port 64038 failed: Connection refused
   *   Trying 127.0.0.1:8080...
   * Connected to localhost (127.0.0.1) port 8080
   > POST /paste HTTP/1.1
   > Host: localhost:8080
   > User-Agent: curl/8.6.0
   > Accept: */*
   > Content-Length: 25
   > Content-Type: application/x-www-form-urlencoded
   >
   < HTTP/1.1 201 Created
   < Content-Type: text/plain
   < Content-Length: 37
   < Connection: close
   <
   http://localhost:8080/raw/1722133722
   * Closing connection
   ```

4. **Retrieving a Paste**:
   Retrieve the contents of a previously created paste.
   ```sh
   curl http://localhost:8080/raw/1722133722
   ```
   Response:
   ```
   clishae works
   hello world
   ```

These examples showcase how to interact with the server for uploading, deleting, creating, and retrieving pastes. Adjust the URLs and data as needed for your specific use case.
