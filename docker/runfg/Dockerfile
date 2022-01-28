from newsched:latest

COPY run_fg_from_json.py .
# You need to mount fg.json into the container
# i.e docker run -v <input_file>:fg.json
CMD source /root/setup_env.sh && python3 run_fg_from_json.py fg.json