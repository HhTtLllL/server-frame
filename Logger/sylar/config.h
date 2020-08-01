#ifndef __SYLAR_CONFIG_H__
#define __SYLAR_CONFIG_H__

#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "./log.h"


namespace sylar
{

//配置变量的基类
class ConfigVarBase{
	
public:
	typedef std::shared_ptr<ConfigVarBase> ptr;

	ConfigVarBase(const std::string& name, const std::string& description = "")
		:m_name(name),
		m_description(description) {

		}

	virtual ~ConfigVarBase() {}

	//返回配置参数与的名称
	const std::string& getName() const { return m_name; }
	//返回配置参数的描述
	const std::string& getDescription() const { return m_description; }

	//转成字符串
	virtual std::string toString() = 0; //序列化
	//从字符串初始化值
	virtual bool fromString (const std::string& val) = 0;//反序列化


protected:
	std::string m_name;//配置名称
	std::string m_description; //配置描述
};

//配置项的具体实现类
template<class T>
class ConfigVar : public ConfigVarBase{

public:
	typedef std::shared_ptr<ConfigVar> ptr;

	ConfigVar(const std::string& name, const T& default_value,const std::string& description = "")
		: ConfigVarBase(name,description), m_val(default_value){
		
		}

	std::string toString () override{
	
		try{
			//在这里捕捉类型转化的异常 把m_val 类型转化为string 类型
			return boost::lexical_cast<std::string>(m_val); //类型转化
		}catch(std::exception& e){
			SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "configVar::toString exception" << e.what() << " convert: " << typeid(m_val).name() << "to string";

		}

		return "";
	}


	bool fromString(const std::string& val) override {
		
		try{
			//把string 类型转换为我们需要的类型 模板T 类型
			m_val = boost::lexical_cast<T>(val);

		}catch(std::exception& e){
			SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "configVar::toString exception" << e.what() << " convert: " << typeid(m_val).name() << "to string";

		}


		return false;
	}

	const T getValue() const { return m_val; }
	void setValue(const T& val) { m_val = val ; } 
private:
	T m_val;





};

//ConfigVar 的集合类
class Config{

public:

	typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

	template<class T>
	static typename ConfigVar<T>::ptr Lookup(const std::string& name,
			const T& default_value, const std::string& description = ""){
		auto tmp = Lookup<T>(name);
		if(tmp){
			SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "Lookup name" << name << "exists";
			return tmp;
		}

		if(name.find_first_not_of("abcdefghigklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._0123456789") != std::string::npos){
			SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name invalid " << name;
			throw std::invalid_argument(name);
		
		}

		typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value,description));
		s_datas[name] = v;

		return v;
	}

	template<class T>
	static typename ConfigVar<T>::ptr Lookup(const std::string& name){
		auto it = s_datas.find(name);
		if(it == s_datas.end()){
			return nullptr;
		}

		return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
	
	}

private:
	static ConfigVarMap s_datas;

};
















}





#endif
