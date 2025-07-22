FROM arm64v8/gcc:13 AS builder
WORKDIR /app
COPY . .
RUN chmod +x ./scripts/*.sh ./run_ci.sh
RUN ./scripts/lint.sh && ./scripts/build.sh

FROM builder AS ci
RUN ./run_ci.sh

FROM arm64v8/debian:bullseye-slim AS prod
WORKDIR /app
COPY --from=builder /app/build/bin /app/bin
ENTRYPOINT ["/app/bin/main"]