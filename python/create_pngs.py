import os
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

from count_csv_files import count_csv_files
from shell_command import shell_command

head_path="./"

# csv filesのフォルダパス
csv_dir=head_path+"ey_timestep_csvs/"

# outputするフォルダパス
png_dir=head_path+"pngs/"

output_dir="./mp4/"
output_file="output.mp4"

# csvのファイル数の取得
timestep = count_csv_files(csv_dir)

print(str(timestep)+" csv files were found.")

# eyの最大値、最小値の取得
ey_range_file="./csv_files/ey_range.csv"

df=pd.read_csv(ey_range_file,header=None)

print("ey_max="+str(df.iloc[0].max()))
print("ey_min="+str(df.iloc[1].min()))

ey_max=df.iloc[0].max()
ey_min=df.iloc[1].min()

# hzの最大値、最小値の取得
hz_range_file="./csv_files/hz_range.csv"

df=pd.read_csv(hz_range_file,header=None)

print("hz_max="+str(df.iloc[0].max()))
print("hz_min="+str(df.iloc[1].min()))

# hzの最大値、最小値の取得
hz_max=df.iloc[0].max()
hz_min=df.iloc[1].min()

timestep = int(input("input timestep number(>0)."))

for i in range(timestep):
    
    # 6桁固定のiを作り出す
    fmt_i = '{:06d}'.format(i)
    csv_file="./ey_timestep_csvs/ey_timestep_"+fmt_i+".csv"
    
    # data frameに読み込む
    df_ey=pd.read_csv(csv_file,header=None)
    
    # x軸のデータ（1行目）
    xticks = df_ey.iloc[0]

    # データ部分（2行目以降）
    data = df_ey.iloc[1:]

    # DataFrameを転置する（行と列を入れ替える）
    # 折れ線グラフは縦方向に転置し、heatmapは横方向に読み込む

    # figure(全体図)のサイズをfull hdに設定
    fig=plt.figure(figsize=(1920/80,1080/80),dpi=80)

    # 2行1列の1行目
    ax1=fig.add_subplot(2,1,1)
    
    ax1.set_title('Ey Amplitude',{"fontsize":20})
    
    ax1.set_xlabel('x position',{"fontsize":15})
    ax1.set_ylabel('Ey amplitude',{"fontsize":15})
 
    ax1.set_xlim(left=0)
    ax1.set_xlim(right=len(xticks))

    ax1.set_xticks( np.arange(0, len(xticks), 5) )
    # ax1.set_yticks( np.arange(min , max , 0.0000025) )

    ax1.plot(df_ey.T)

    ax1.set_ylim(top=ey_max)
    ax1.set_ylim(bottom=ey_min)
    
    # 折れ線グラフのプロット
    ax1.plot(df_ey.T,color="orange")
    
    # csv fileの削除
    os.unlink(csv_file)

    # hz成分
    # 2行1列の2行目
    ax2=fig.add_subplot(2,1,2)

    csv_file="./hz_timestep_csvs/hz_timestep_"+fmt_i+".csv"
        
    # data frameに読み込む
    df_hz=pd.read_csv(csv_file,header=None)
    
    # print(df_hz)
    
    # x軸のデータ（1行目）
    xticks = df_hz.iloc[0]

    # データ部分（2行目以降）
    data = df_hz.iloc[1:]
    
    ax2.set_title('Hz Amplitude',{"fontsize":20})
    
    ax2.set_xlabel('x position',{"fontsize":15})
    ax2.set_ylabel('Hz amplitude',{"fontsize":15})
 
    ax2.set_xlim(left=0)
    ax2.set_xlim(right=len(xticks))

    ax2.set_xticks( np.arange(0, len(xticks), 5) )
    
    ax2.set_ylim(top=hz_max)
    ax2.set_ylim(bottom=hz_min)

    ax2.plot(df_hz.T,color="blue")

    # heatmapにもx軸を設定する。間隔はxticklabels=5で設定する。
    # heatmap=sns.heatmap(data, xticklabels=5, yticklabels=False,cmap='coolwarm',cbar=False,center=0.0)
    # # ax2.xaxis.set_major_locator(ticker.MultipleLocator(5)) 
    # heatmap.set_xlabel('x position' , {"fontsize":15})
    
    plt.suptitle("timestep="+str(i),fontsize=25)
    # plt.tight_layout()

    # ファイルを保存
    plt.savefig(png_dir+"png_"+fmt_i+".png")
    
    # saveの表示
    print(png_dir+"png_"+fmt_i+".png"+"/"+str(timestep)+" was created.")
    
    # csv fileの削除
    os.unlink(csv_file)
    
    # メモリの解放
    plt.clf()
    plt.close()

# 最後は下記のコマンドで動画ができる。
# ffmpeg -r 10 -i ../pngs/png_%06d.png -c:v libx264 -pix_fmt yuv420p output.mp4


# 後処理
command="rm ./mp4/output.mp4"
shell_command(command)

command="ffmpeg -r 10 -i ./pngs/png_%06d.png -c:v libx264 -pix_fmt yuv420p ./mp4/output.mp4"
shell_command(command)    
    
command="rm -rf ./pngs/*.png"
shell_command(command)

command="vlc ./mp4/output.mp4"
shell_command(command)

command="rm -rf ./ey_timestep_csvs/*.csv"
shell_command(command)

command="rm -rf ./hz_timestep_csvs/*.csv"
shell_command(command)
