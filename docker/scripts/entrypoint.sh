#!/bin/bash

# Add local user
# Either use the LOCAL_USER_ID if passed in at runtime or fallback

USER_ID=${LOCAL_USER_ID:-1000}
USER_NAME=${LOCAL_USER_NAME:-developer}

echo "Starting with UID : ${USER_ID} (${USER_NAME})"
useradd --shell /bin/bash -u ${USER_ID} -o -c "" -m ${USER_NAME}
echo "${USER_NAME}:${USER_NAME}" | chpasswd
export HOME=/home/${USER_NAME}
export USER=${USER_NAME}

if [[ -z "$@" ]]; then
	bash -c "su ${USER_NAME}"
else
	bash -c "su ${USER_NAME} -c \"$@\""
fi
