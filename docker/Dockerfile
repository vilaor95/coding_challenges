FROM archlinux:base-devel

RUN pacman -Sy && pacman -S --noconfirm \
	    vim

RUN echo "root:root" | chpasswd

COPY ./scripts/entrypoint.sh /usr/local/bin/entrypoint.sh
RUN chmod +x /usr/local/bin/entrypoint.sh
ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
