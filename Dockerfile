
FROM gcc:13

WORKDIR /app

COPY . .

RUN g++ -std=c++17 -pthread final.cpp -o inventory_server

EXPOSE 10000

CMD ["./inventory_server"]
