package com.cloudnotes.service.service.impl;

import com.cloudnotes.service.common.Result;
import com.cloudnotes.service.entity.Catalogue;
import com.cloudnotes.service.entity.Note;
import com.cloudnotes.service.entity.TextBlock;
import com.cloudnotes.service.entity.User;
import com.cloudnotes.service.mapper.*;
import com.cloudnotes.service.service.UserService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

@Service
public class UserServiceImpl implements UserService {
    @Autowired
    UserMapper userMapper;
    @Autowired
    CatalogMapper catalogMapper;
    @Autowired
    NoteMapper noteMapper;
    @Autowired
    TextBlockMapper textBlockMapper;
    @Autowired
    ShareNoteMapper shareNoteMapper;
    @Override
    public Result login(String userName, String password) {
        User user=userMapper.findByUserNameAndPassword(userName,password);
        if(user!=null)return new Result("0","登录成功",user);
        else return new Result("1","用户名或密码错误",null);
    }
    @Override
    public Result register(User user) {
        User temp=userMapper.findByUserName(user.getUserName());
        if(temp!=null)return new Result("1","用户名已存在",null);
        else{
            User temp2= userMapper.save(user);
            // 用户新创建时，就为用户创建根目录
            Catalogue catalogue=new Catalogue();
            catalogue.setName("个人文档");
            catalogue.setUserId(temp2.getId());
            catalogue.setFatherId(0);
            catalogMapper.save(catalogue);
            return new Result("0","注册成功",null);
        }
    }
    @Override
    public Result update(User user) {
        userMapper.save(user);
        return new Result("0","修改成功",null);
    }
    @Override
    @Transactional
    public Result logout(int id) {
        userMapper.deleteById(id);
        catalogMapper.deleteByUserId(id);
        noteMapper.deleteByUserId(id);
        textBlockMapper.deleteByUserId(id);
        shareNoteMapper.deleteByUserId(id);
        return new Result("0","注销成功",null);
    }
}
