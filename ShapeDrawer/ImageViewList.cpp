#include "ImageViewList.h"

ImageViewList::ImageViewList(QWidget *parent) : QWidget(parent)
{
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(50,50,50)); //设置背景黑色
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    DispLayout = new QGridLayout();
    DispLayout->setSpacing(2);//间隔
    DispLayout->setMargin(2);//间隙
    this->setLayout(DispLayout);
}
//动态设定显示区域
void ImageViewList::SetViewCount(int num)
{
    if(num<0)return;
    if(num>ViewList.size())//新增显示窗口
    {
        //清空布局
        for(int i=0;i<ViewList.size();i++)
        {
            DispLayout->removeWidget(ViewList[i]);
        }
        //新增一个
        for(int i=ViewList.size();i<num;i++)
        {
            ViewList.push_back(new QGraphicsViews);
            connect(ViewList[ViewList.size()-1],&QGraphicsViews::Expand,this,&ImageViewList::Expand);
        }
        //重新布局
        int count=ViewList.size();
        int ct=count<=4?2:(count<=6?3:4);//动态调整布局
        for(int i=0;i<ViewList.size();i++)
        {
            DispLayout->addWidget(ViewList[i],i/ct,i%ct,1,1);
        }
    }else if(num<ViewList.size())//删除
    {
        //清空布局
        for(int i=0;i<ViewList.size();i++)
        {
            DispLayout->removeWidget(ViewList[i]);
        }
        //删除最后一个
        int Rnum=ViewList.size()-num;
        for (int i=0;i<Rnum;i++)
        {
            disconnect(ViewList[ViewList.size()-1],&QGraphicsViews::Expand,this,&ImageViewList::Expand);
            delete ViewList[ViewList.size()-1];
            ViewList.removeAt(ViewList.size()-1);
        }
        //重新布局
        int count=ViewList.size();
        int ct=count<=4?2:(count<=6?3:4);//动态调整布局
        for(int i=0;i<ViewList.size();i++)
        {
            DispLayout->addWidget(ViewList[i],i/ct,i%ct,1,1);
        }
    }
}
//单独放大某一个窗口 使用隐藏的方式、不显示的隐藏后布局自适应。
void ImageViewList::Expand(QWidget *ptr)
{
    if(Ex)
    {
        for(auto item:ViewList)
        {
            item->setVisible(true);
        }
        Ex=false;
    }else
    {
        QGraphicsViews* tmp=(QGraphicsViews*)ptr;
        for(auto item:ViewList)
        {
            if(item!=tmp)
            {
                item->setVisible(false);
            }
        }
        Ex=true;
    }
}
//显示信息
void ImageViewList::DispInfo(int CamNum,int statue,int Result,QString str,bool All)
{
    if(CamNum<1||CamNum>ViewList.size())
        return;

    if(All)
    {
        ViewList[CamNum-1]->DispInfo(statue);//相机连接状态
    }else
    {
        ViewList[CamNum-1]->DispInfo(statue,Result,str,All);//相机连接状态
    }

}
//显示图片
void ImageViewList::DispImage(int CamNum, QImage &img)
{
    if(CamNum<1||CamNum>ViewList.size())
        return;
    ViewList[CamNum-1]->DispImage(img);
}

