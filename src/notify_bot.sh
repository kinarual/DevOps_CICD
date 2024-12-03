#!/bin/bash

TG_BOT_TOKEN="insert here"
TG_USER_ID="insert here"

if [ "$CI_JOB_STATUS" == "success" ]; then
  JOB_STATUS="$CI_JOB_STAGE ✅"
  RESULT="success"
else
  JOB_STATUS="$CI_JOB_STAGE 🚫"
  RESULT="fail"
fi

MESSAGE="По проекту $CI_PROJECT_NAME
В ветке $CI_COMMIT_BRANCH
Выполнился этап CI/CD: $CI_JOB_NAME ($JOB_STATUS)
Дата: $CI_COMMIT_TIMESTAMP
Комментарий: $CI_COMMIT_MESSAGE
Посмотреть историю и выполнить deploy: https://repos.21-school.ru/students/DO6_CICD.ID_356283/glyndaja_student.21_school.ru/DO6_CICD-1/-/pipelines"

IMAGE_URL_BUILD="https://media1.tenor.com/m/qsniPtwh7PoAAAAC/spongebob-patrick.gif"
IMAGE_URL_STYLE="https://media1.tenor.com/m/9_aejokMHsYAAAAd/savage-cat.gif"
IMAGE_URL_TEST="https://media1.tenor.com/m/jV37gl3zZisAAAAC/perfect-match.gif"
IMAGE_URL_DEPLOY="https://media1.tenor.com/m/ZBt12E1nwqQAAAAC/deploy-parksandrec.gif"

ESCAPED_MESSAGE=$(echo "${MESSAGE}" | sed 's/_/\\_/g; s/\*/\\*/g; s/`/\\`/g')

curl -s -X POST https://api.telegram.org/bot${TG_BOT_TOKEN}/sendMessage \
  -d chat_id=${TG_USER_ID} \
  -d text="${ESCAPED_MESSAGE}" \
  -d parse_mode="Markdown"

if [ "$CI_JOB_NAME" == "build_job" ] && [ "$RESULT" == "success" ]; then
    curl -s -X POST "https://api.telegram.org/bot${TG_BOT_TOKEN}/sendVideo" -F chat_id="${TG_USER_ID}" -F video="${IMAGE_URL_BUILD}"
elif [ "$CI_JOB_NAME" == "style_job" ] && [ "$RESULT" == "success" ]; then 
    curl -s -X POST "https://api.telegram.org/bot${TG_BOT_TOKEN}/sendVideo" -F chat_id="${TG_USER_ID}" -F video="${IMAGE_URL_STYLE}"
elif [ "$CI_JOB_NAME" == "test_job" ] && [ "$RESULT" == "success" ]; then
    curl -s -X POST "https://api.telegram.org/bot${TG_BOT_TOKEN}/sendVideo" -F chat_id="${TG_USER_ID}" -F video="${IMAGE_URL_TEST}"
elif [ "$CI_JOB_NAME" == "deploy_job" ] && [ "$RESULT" == "success" ]; then
    curl -s -X POST "https://api.telegram.org/bot${TG_BOT_TOKEN}/sendVideo" -F chat_id="${TG_USER_ID}" -F video="${IMAGE_URL_DEPLOY}"
fi