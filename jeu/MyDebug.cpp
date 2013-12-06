#include "MyDebug.h"


MyDebug::MyDebug() : m_debugMode(0)
{
}

MyDebug::~MyDebug()
{
}

void MyDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
	glBegin(GL_LINES);
	glColor3f(fromColor.getX(), fromColor.getY(),
		fromColor.getZ());
	glVertex3f(from.getX(), from.getY(), from.getZ());
	glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
	glVertex3f(to.getX(), to.getY(), to.getZ());
	glEnd();
}

void MyDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	drawLine(from, to, color, color);
}

void  MyDebug::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	{
		//btVector3 to=pointOnB+normalOnB*distance;
		//const btVector3&from = pointOnB;
		//glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);   

		//GLDebugDrawer::drawLine(from, to, color);

		//glRasterPos3f(from.x(),  from.y(),  from.z());
		//char buf[12];
		//sprintf(buf," %d",lifeTime);
		//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
	}
}



void MyDebug::reportErrorWarning(const char* warningString)
{
	printf (warningString);
}

void MyDebug::draw3dText(const btVector3& location, const char* textString)
{
}

void MyDebug::setDebugMode(int debugMode)
{
	m_debugMode = debugMode;
}

int MyDebug::getDebugMode() const
{
	return m_debugMode;
}