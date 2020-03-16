FROM ahmad88me/tadahdtentity:latest

WORKDIR /app

COPY scripts /app/scripts
COPY src /app/src
COPY Makefile /app/Makefile
COPY .git /app/.git
COPY test.ttl /app/test.ttl
COPY test_files /app/test_files
CMD ["sh", "scripts/start.sh"]
