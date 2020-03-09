FROM ahmad88me/tadahdtentity:latest

WORKDIR /app

COPY scripts /app/scripts
COPY src /app/src
COPY Makefile /app/Makefile
COPY .git /app/.git

CMD ["sh", "scripts/start.sh"]
