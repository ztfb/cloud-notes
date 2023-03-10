package com.cloudnotes.service.mapper;

import com.cloudnotes.service.entity.User;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

@Repository
public interface UserMapper extends JpaRepository<User,Integer> {
    // 查询语句
    @Query(value="select * from user where user.user_name=:userName" ,nativeQuery=true)
    public User findByUserName(@Param("userName") String userName);
    @Query(value="select * from user where user.user_name=:userName and user.password=:password" ,nativeQuery=true)
    public User findByUserNameAndPassword(@Param("userName") String userName,@Param("password") String password);
}
