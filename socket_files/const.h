enum queryCode {
	FILE_EXISTS_REQUEST,
	FILE_EXISTS_RESPONCE,
	FILE_SIZE_REQUEST,
	FILE_SIZE_RESPONCE
};

struct packet {
    queryCode code;
    int contentLength;
    char * content;
};
