import pydub
import os

SELFPATH="./sounds/"
INPATH="source"
OUTPATH="sourceConverted"

if not os.path.exists(SELFPATH+OUTPATH):
    os.mkdir(SELFPATH+OUTPATH)

for filename in os.listdir(SELFPATH+INPATH):
    print(filename)
    newfilename = filename[:-4]
    if(filename.endswith(".mp3")):
        sound = pydub.AudioSegment.from_mp3(SELFPATH+INPATH+"/"+filename)
    elif (filename.endswith(".wav")):
        sound = pydub.AudioSegment.from_wav(SELFPATH+INPATH+"/"+filename)
    else:
        sound = pydub.AudioSegment.from_raw(SELFPATH+INPATH+"/"+filename)
    sound.export(SELFPATH+OUTPATH+f"/{newfilename}.wav", format="wav", parameters=["-ar","32K","-ac", "1"])


    