// ============================================================================
//			Copyright (c) 2013 Prescient Technologies Pvt Ltd. India
//								All Rights Reserved
// ============================================================================

#ifndef  _COMMON_BLOCK_INTERFACE_H_
#define  _COMMON_BLOCK_INTERFACE_H_

namespace CommonUtil
{

//  ***** DESCRIPTION of class BlockInterface
//	This class is used in "Parallel Processing Utility".
//	This is an interface class and it is empty as the utility just holds
//	an instance of this class and passes it to application (that uses this utility) 
//	for processing.
//
//  ***** HOW TO USE THIS CLASS AT APPLICATION SIDE ?
//	Inherit a class from this class. The child class should hold the data to be processed

class BlockInterface
{
protected:

	// Flag indicating weather block given for processing or not
	bool m_processing;

	// Flag indicates that thread has processed the block
	bool m_processed;

	// Flag to know whether the block is checked
	bool m_checked;

public:

	// Constructor
	BlockInterface(){}

	// Destructor
	~BlockInterface(){}

	void SetProcessing()
	{
		m_processing = true;
		m_processed = false;
	}

	void SetProcessed()
	{
		m_processing = false;
		m_processed = true;
	}

	void SetUnprocessed()
	{
		m_processing = false;
		m_processed = false;
	}

	bool IsProcessing()const{ return m_processing; }

	bool IsProcessed()const{ return m_processed; }

	void SetChecked(bool check){ m_checked = check; }

	bool IsChecked()const{ return m_checked; }

};
}

#endif